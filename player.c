#include "player.h"

void rr_player_init(rrPlayer* player, rrGrid* grid) {
    rr_point_set(&player->position, 0, 0);
    player->score = 0;
    player->lives = 0;

    player->_grid = grid;
}

int rr_player_set_pos(rrPlayer* player, rrPoint* position) {
    if (!rr_grid_position_is_valid(player->_grid, position)) {
        return 0;
    }

    rr_point_copy(&player->position, position);

    rr_grid_set_cell_type(player->_grid, &player->position, RR_CELL_MOUSE);

    return 1;
}

int rr_player_push_horizontal(rrPlayer* player, rrPoint* target, int dir) {
    rrCellType end_cell_type;
    rrPoint end_cell;
    rr_point_copy(&end_cell, target);

    /* find the first non block tile along the direction*/
    do {
        end_cell.x += dir;
        end_cell_type = rr_grid_get_cell_type(player->_grid, &end_cell);
    }
    while (end_cell_type == RR_CELL_BLOCK);

    /* blocks along this vector are against a wall and cannot be moved. */
    if (end_cell_type == RR_CELL_WALL)
        return 0;

    /* push all the blocks over */
    while (end_cell.x != target->x) {
        rr_grid_set_cell_type(player->_grid, &end_cell, RR_CELL_BLOCK);
        end_cell.x -= dir;
    }

    return 1;
}

int rr_player_push_vertical(rrPlayer* player, rrPoint* target, int dir) {
    rrCellType end_cell_type;
    rrPoint end_cell;
    rr_point_copy(&end_cell, target);

    /* find the first non block tile along the direction*/
    do {
        end_cell.y += dir;
        end_cell_type = rr_grid_get_cell_type(player->_grid, &end_cell);
    }
    while (end_cell_type == RR_CELL_BLOCK);

    /* blocks along this vector are against a wall and cannot be moved. */
    if (end_cell_type == RR_CELL_WALL)
        return 0;

    /* push all the blocks over */
    while (end_cell.y != target->y) {
        rr_grid_set_cell_type(player->_grid, &end_cell, RR_CELL_BLOCK);
        end_cell.y -= dir;
    }

    return 1;
}

int rr_player_push(rrPlayer* player, rrPoint* target) {
    rrPoint delta;
    rr_point_sub(&delta, target, &player->position);

    if (delta.x)
        return rr_player_push_horizontal(player, target, delta.x);
    else if (delta.y)
        return rr_player_push_vertical(player, target, delta.y);
    else return 0;
}


int rr_player_move(rrPlayer* player, rrPoint* delta) {
    rrCellType targetCellType;
    int can_move = 1;
    rrPoint target;
    rr_point_add(&target, &player->position, delta);

    if (!rr_grid_position_is_valid(player->_grid, &target))
        return 0;

    targetCellType = rr_grid_get_cell_type(player->_grid, &target);

    if (targetCellType == RR_CELL_WALL)
        can_move = 0;

    else if (targetCellType == RR_CELL_BLOCK)
        can_move = rr_player_push(player, &target);

    if (can_move){
        rr_grid_set_cell_type(player->_grid, &player->position, RR_CELL_EMPTY);
        rr_player_set_pos(player, &target);
    }

    return can_move;
}