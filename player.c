#include "player.h"

void rr_player_init(rrPlayer* player, rrGrid* grid) {
    rr_entity_init(&player->entity, RR_ENTITY_PLAYER);
    player->_grid = grid;
    player->score = 0;
    player->lives_remaining = 0;

}

int rr_player_push_horizontal(rrPlayer* player, rrPoint* target, int dir) {
    rrEntity* end_cell_entity;
    rrPoint end_cell_pos;
    rr_point_copy(&end_cell_pos, target);

    /* find the first non block tile along the direction*/
    do {
        end_cell_pos.x += dir;
        end_cell_entity = rr_grid_get_cell(player->_grid, &end_cell_pos);
    }
    while (end_cell_entity != NULL && end_cell_entity->type == RR_ENTITY_BLOCK);

    /* blocks along this vector are against an immovable object and cannot be moved. */
    if (rr_grid_cell_is_blocked(player->_grid, &end_cell_pos))
        return 0;

    /* push all the blocks over */
    while (end_cell_pos.x != target->x) {
        rrPoint src_cell;
        rr_point_set(&src_cell, end_cell_pos.x - dir, end_cell_pos.y);
        rr_grid_update_entity_position(player->_grid, rr_grid_get_cell(player->_grid, &src_cell), &end_cell_pos);
        end_cell_pos.x -= dir;
    }

    return 1;
}

int rr_player_push_vertical(rrPlayer* player, rrPoint* target, int dir) {
    rrEntity* end_cell_entity;
    rrPoint end_cell_pos;
    rr_point_copy(&end_cell_pos, target);

    /* find the first non block tile along the direction*/
    do {
        end_cell_pos.y += dir;
        end_cell_entity = rr_grid_get_cell(player->_grid, &end_cell_pos);
    }
    while (end_cell_entity != NULL && end_cell_entity->type == RR_ENTITY_BLOCK);

    /* blocks along this vector are against an immovable object and cannot be moved. */
    if (rr_grid_cell_is_blocked(player->_grid, &end_cell_pos))
        return 0;

    /* push all the blocks over */
    while (end_cell_pos.y != target->y) {
        rrPoint src_cell;
        rr_point_set(&src_cell, end_cell_pos.x, end_cell_pos.y - dir);
        rr_grid_update_entity_position(player->_grid, rr_grid_get_cell(player->_grid, &src_cell), &end_cell_pos);
        end_cell_pos.y -= dir;
    }

    return 1;
}

int rr_player_push(rrPlayer* player, rrPoint* target) {
    rrPoint delta;
    rr_point_sub(&delta, target, &player->entity.position);
    int pushed = 0;

    if (delta.x)
        pushed = rr_player_push_horizontal(player, target, delta.x);
    else if (delta.y)
        pushed =  rr_player_push_vertical(player, target, delta.y);

    if (pushed) {
        rr_grid_update_entity_position(player->_grid, &player->entity, target);
    }

    return pushed;
}

void rr_player_move(rrPlayer* player, rrPoint* delta) {
    rrEntity* target_cell;
    rrPoint target;
    rr_point_add(&target, &player->entity.position, delta);

    if (!rr_grid_position_is_valid(player->_grid, &target))
        return;

    target_cell = rr_grid_get_cell(player->_grid, &target);

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
    }
}

void rr_player_kill(rrPlayer* player) {
    player->entity.status = RR_STATUS_KILLED;
    rr_grid_clear_position(player->_grid, &player->entity.position);
    rr_entity_set_invalid_position(&player->entity);
}
