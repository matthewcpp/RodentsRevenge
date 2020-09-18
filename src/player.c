#include "player.h"
#include "enemy.h"

#include <assert.h>

#define RR_PLAYER_INPUT_REPEAT_TIME 1000
#define RR_PLAYER_HOLE_STUCK_TIME 10000

void rr_player_init(rrPlayer* player, rrGrid* grid, rrInput* input) {
    rr_entity_init(&player->entity, RR_ENTITY_PLAYER);
    player->_grid = grid;
    player->_input = input;
    player->score = 0;
    player->lives_remaining = 2;
    player->_killer_entity = NULL;
    player->death_animation = NULL;
    player->time_stuck = 0;
}

int rr_player_input_button_active(rrInput* input, rrInputButton button) {
    return rr_input_button_down(input, button) || rr_input_button_held_time(input, button) > RR_PLAYER_INPUT_REPEAT_TIME;
}

void rr_player_update_active(rrPlayer* player) {
    rrPoint delta = {0, 0};
    if (rr_player_input_button_active(player->_input, RR_INPUT_BUTTON_LEFT))
        delta.x = -1;
    else if (rr_player_input_button_active(player->_input, RR_INPUT_BUTTON_RIGHT))
        delta.x = 1;
    else if (rr_player_input_button_active(player->_input, RR_INPUT_BUTTON_UP))
        delta.y = -1;
    else if (rr_player_input_button_active(player->_input, RR_INPUT_BUTTON_DOWN))
        delta.y = 1;

    if (!rr_point_equals(&delta, rr_point_zero()))
        rr_player_move(player, &delta);
}

void rr_player_update_dying(rrPlayer* player, int time) {
    rr_animation_update(player->death_animation, time);

    if (rr_animation_complete(player->death_animation)) {
        rrPoint position;
        rr_point_copy(&position, &player->entity.position);

        rr_entity_remove_from_grid(&player->entity, player->_grid);
        player->entity.status = RR_STATUS_KILLED;

        rr_entity_place_in_grid_cell(player->_killer_entity, player->_grid, &position);
        player->_killer_entity->status = RR_STATUS_ACTIVE;
        player->_killer_entity = NULL;
    }
}

void rr_player_update_stuck(rrPlayer* player, int time) {
    player->time_stuck += time;

    if (player->time_stuck >= RR_PLAYER_HOLE_STUCK_TIME) {
        player->entity.status = RR_STATUS_ACTIVE;
        player->time_stuck = 0;
    }
}

void rr_player_update(rrPlayer* player, int time) {
    if (player->entity.status == RR_STATUS_ACTIVE)
        rr_player_update_active(player);
    else if (player->entity.status == RR_STATUS_DYING)
        rr_player_update_dying(player, time);
    else if (player->entity.status == RR_STATUS_STUCK)
        rr_player_update_stuck(player, time);
}

int rr_player_push(rrPlayer* player, rrPoint* target) {
    rrPoint dir, end_cell_pos;
    rrEntity* end_cell_entity = NULL;
    rr_point_sub(&dir, target, &player->entity.position);
    rr_point_copy(&end_cell_pos, target);

    /* find the first non block tile along the direction*/
    do {
        rr_point_add(&end_cell_pos, &end_cell_pos, &dir);
        end_cell_entity = rr_grid_get_entity_at_position(player->_grid, &end_cell_pos);
    }
    while (end_cell_entity != NULL && end_cell_entity->type == RR_ENTITY_BLOCK);

    if (end_cell_entity) {
        /* blocks along this vector are against an immovable object and cannot be moved. */
        if (end_cell_entity->type == RR_ENTITY_WALL)
            return 0;
        else if (end_cell_entity->type == RR_ENTITY_ENEMY) {
            /* we can only push the block if the enemy was able to move to a safe square, otherwise its pinned. */
            int enemy_evaded = rr_enemy_move((rrEnemy*)end_cell_entity);
            if (!enemy_evaded)
                return 0;
        }
        else if (end_cell_entity->type == RR_ENTITY_CHEESE) {
            rr_grid_destroy_basic_entity(player->_grid, end_cell_entity);
        }
        else if (end_cell_entity->type == RR_ENTITY_HOLE) {
            rr_point_sub(&end_cell_pos, &end_cell_pos, &dir);
            rr_grid_destroy_basic_entity(player->_grid, rr_grid_get_entity_at_position(player->_grid, &end_cell_pos));
        }
    }

    /* push all the blocks over */
    while (!rr_point_equals(&end_cell_pos, target)) {
        rrPoint src_cell;
        rr_point_sub(&src_cell, &end_cell_pos, &dir);
        rr_entity_move_to_grid_cell(rr_grid_get_entity_at_position(player->_grid, &src_cell), player->_grid, &end_cell_pos);
        rr_point_sub(&end_cell_pos, &end_cell_pos, &dir);
    }

    rr_entity_move_to_grid_cell(&player->entity, player->_grid, target);

    return 1;
}

void rr_player_move(rrPlayer* player, rrPoint* delta) {
    rrEntity* target_cell_entity;
    rrPoint target;
    rr_point_add(&target, &player->entity.position, delta);

    if (!rr_grid_position_is_valid(player->_grid, &target))
        return;

    target_cell_entity = rr_grid_get_entity_at_position(player->_grid, &target);

    if (target_cell_entity == NULL) {
        rr_entity_move_to_grid_cell(&player->entity, player->_grid, &target);
        return;
    }

    switch (target_cell_entity->type) {
        case RR_ENTITY_BLOCK:
            rr_player_push(player, &target);
            break;

        case RR_ENTITY_ENEMY:
            rr_enemy_suspend((rrEnemy*)target_cell_entity);
            rr_entity_move_to_grid_cell(&player->entity, player->_grid, &target);
            rr_player_kill(player, target_cell_entity);
            break;

        case RR_ENTITY_CHEESE:
            player->score += 100;
            rr_grid_destroy_basic_entity(player->_grid, target_cell_entity);
            rr_entity_move_to_grid_cell(&player->entity, player->_grid, &target);
            break;

        case RR_ENTITY_HOLE:
            player->entity.status = RR_STATUS_STUCK;
            rr_grid_destroy_basic_entity(player->_grid, target_cell_entity);
            rr_entity_move_to_grid_cell(&player->entity, player->_grid, &target);
            break;

        default:
            break;
    }
}

void rr_player_kill(rrPlayer* player, rrEntity* entity) {
    assert(entity->status == RR_STATUS_SUSPENDED);

    player->entity.status = RR_STATUS_DYING;
    rr_animation_reset(player->death_animation);

    player->_killer_entity = entity;
}
