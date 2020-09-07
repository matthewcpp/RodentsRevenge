#include "player.h"

void rr_player_init(rrPlayer* player, rrGrid* grid) {
    rr_entity_init(&player->entity, RR_ENTITY_PLAYER);
    player->_grid = grid;
    player->score = 0;
    player->lives_remaining = 0;

}

int rr_player_push_horizontal(rrPlayer* player, rrPoint* target, int dir) {
    rrType end_cell_type;
    rrPoint end_cell;
    rr_point_copy(&end_cell, target);

    /* find the first non block tile along the direction*/
    do {
        end_cell.x += dir;
        end_cell_type = rr_grid_get_cell(player->_grid, &end_cell)->type;
    }
    while (end_cell_type == RR_CELL_BLOCK);

    /* blocks along this vector are against an immovable object and cannot be moved. */
    if (rr_grid_cell_is_blocked(player->_grid, &end_cell))
        return 0;

    /* push all the blocks over */
    while (end_cell.x != target->x) {
        rr_grid_set_cell_type(player->_grid, &end_cell, RR_CELL_BLOCK);
        end_cell.x -= dir;
    }

    rr_grid_set_cell_type(player->_grid, &end_cell, RR_CELL_EMPTY);

    return 1;
}

int rr_player_push_vertical(rrPlayer* player, rrPoint* target, int dir) {
    rrType end_cell_type;
    rrPoint end_cell;
    rr_point_copy(&end_cell, target);

    /* find the first non block tile along the direction*/
    do {
        end_cell.y += dir;
        end_cell_type = rr_grid_get_cell(player->_grid, &end_cell)->type;
    }
    while (end_cell_type == RR_CELL_BLOCK);

    /* blocks along this vector are against an immovable object and cannot be moved. */
    if (rr_grid_cell_is_blocked(player->_grid, &end_cell))
        return 0;

    /* push all the blocks over */
    while (end_cell.y != target->y) {
        rr_grid_set_cell_type(player->_grid, &end_cell, RR_CELL_BLOCK);
        end_cell.y -= dir;
    }

    rr_grid_set_cell_type(player->_grid, &end_cell, RR_CELL_EMPTY);

    return 1;
}

int rr_player_push(rrPlayer* player, rrPoint* target) {
    rrPoint delta;
    rr_point_sub(&delta, target, &player->entity.position);

    if (delta.x)
        return rr_player_push_horizontal(player, target, delta.x);
    else if (delta.y)
        return rr_player_push_vertical(player, target, delta.y);
    else return 0;
}


int rr_player_move(rrPlayer* player, rrPoint* delta) {
    rrType targetCellType;
    int can_move = 1;
    rrPoint target;
    rr_point_add(&target, &player->entity.position, delta);

    if (!rr_grid_position_is_valid(player->_grid, &target))
        return 0;

    targetCellType = rr_grid_get_cell(player->_grid, &target)->type;

    if (targetCellType == RR_CELL_WALL)
        can_move = 0;

    else if (targetCellType == RR_CELL_BLOCK)
        can_move = rr_player_push(player, &target);

    if (can_move){
        rr_grid_set_cell_blocked(player->_grid, &player->entity.position, 0);
        rr_point_copy(&player->entity.position, &target);
        rr_grid_set_cell_blocked(player->_grid, &player->entity.position, 1);
    }

    return can_move;
}
