#include "player.h"
#include "enemy.h"

#define RR_PLAYER_INPUT_REPEAT_TIME 1000

void rr_player_init(rrPlayer* player, rrGrid* grid, rrInput* input) {
    rr_entity_init(&player->entity, RR_ENTITY_PLAYER);
    player->_grid = grid;
    player->_input = input;
    player->score = 0;
    player->lives_remaining = 0;
}

int rr_player_input_button_active(rrInput* input, rrInputButton button) {
    return rr_input_button_down(input, button) || rr_input_button_held_time(input, button) > RR_PLAYER_INPUT_REPEAT_TIME;
}

void rr_player_update(rrPlayer* player) {
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
            rr_enemy_move((rrEnemy*)end_cell_entity);
        }
        else if (end_cell_entity->type == RR_ENTITY_CHEESE) {
            rr_grid_clear_position(player->_grid, &end_cell_entity->position);
        }
    }

    /* push all the blocks over */
    while (!rr_point_equals(&end_cell_pos, target)) {
        rrPoint src_cell;
        rr_point_sub(&src_cell, &end_cell_pos, &dir);
        rr_grid_update_entity_position(player->_grid, rr_grid_get_entity_at_position(player->_grid, &src_cell), &end_cell_pos);
        rr_point_sub(&end_cell_pos, &end_cell_pos, &dir);
    }

    rr_grid_update_entity_position(player->_grid, &player->entity, target);

    return 1;
}

void rr_player_move(rrPlayer* player, rrPoint* delta) {
    rrEntity* target_cell;
    rrPoint target;
    rr_point_add(&target, &player->entity.position, delta);

    if (!rr_grid_position_is_valid(player->_grid, &target))
        return;

    target_cell = rr_grid_get_entity_at_position(player->_grid, &target);

    if (target_cell == NULL) {
        rr_grid_update_entity_position(player->_grid, &player->entity, &target);
        return;
    }

    switch (target_cell->type) {
        case RR_ENTITY_BLOCK:
            rr_player_push(player, &target);
            break;

        case RR_ENTITY_ENEMY:
            rr_player_kill(player);
            break;

        case RR_ENTITY_CHEESE:
            player->score += 100;
            rr_grid_clear_position(player->_grid, &target);
            rr_grid_update_entity_position(player->_grid, &player->entity, &target);
            break;

        default:
            break;
    }
}

void rr_player_kill(rrPlayer* player) {
    player->entity.status = RR_STATUS_KILLED;
    rr_grid_clear_position(player->_grid, &player->entity.position);
    rr_entity_set_invalid_position(&player->entity);
}
