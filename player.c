#include "player.h"

void rr_player_init(rrPlayer* player, rrGrid* grid) {
    player->score = 0;
    player->pos_x = 0;
    player->pos_y = 0;
    player->lives = 0;

    player->_grid = grid;
}

int rr_player_set_pos(rrPlayer* player, int x, int y) {
    if (!rr_grid_position_is_valid(player->_grid, x, y)) {
        return 0;
    }

    player->pos_x = x;
    player->pos_y = y;

    rr_grid_set_cell_type(player->_grid, player->pos_x, player->pos_y, RR_CELL_MOUSE);

    return 1;
}

int rr_player_push_horizontal(rrPlayer* player, int target_x, int target_y, int dir) {
    int end_x = target_x;
    rrCellType end_cell_type;

    /* find the first non block tile along the direction*/
    do {
        end_x += dir;
        end_cell_type = rr_grid_get_cell_type(player->_grid, end_x, target_y);
    }
    while (end_cell_type == RR_CELL_BLOCK);

    /* blocks along this vector are against a wall and cannot be moved. */
    if (end_cell_type == RR_CELL_WALL)
        return 0;

    /* push all the blocks over */
    while (end_x != target_x) {
        rr_grid_set_cell_type(player->_grid, end_x, target_y, RR_CELL_BLOCK);
        end_x -= dir;
    }

    return 1;
}

int rr_player_push_vertical(rrPlayer* player, int target_x, int target_y, int dir) {
    int end_y = target_y;
    rrCellType end_cell_type;

    /* find the first non block tile along the direction*/
    do {
        end_y += dir;
        end_cell_type = rr_grid_get_cell_type(player->_grid, target_x, end_y);
    }
    while (end_cell_type == RR_CELL_BLOCK);

    /* blocks along this vector are against a wall and cannot be moved. */
    if (end_cell_type == RR_CELL_WALL)
        return 0;

    /* push all the blocks over */
    while (end_y != target_y) {
        rr_grid_set_cell_type(player->_grid, target_x, end_y, RR_CELL_BLOCK);
        end_y -= dir;
    }

    return 1;
}

int rr_player_push(rrPlayer* player, int target_x, int target_y) {
    int delta_x = target_x - player->pos_x;
    int delta_y = target_y - player->pos_y;

    if (delta_x)
        return rr_player_push_horizontal(player, target_x, target_y, delta_x);
    else if (delta_y)
        return rr_player_push_vertical(player, target_x, target_y, delta_y);
    else return 0;
}


int rr_player_move(rrPlayer* player, int delta_x, int delta_y) {
    rrCellType targetCellType;
    int can_move = 1;
    int target_x = player->pos_x + delta_x;
    int target_y = player->pos_y + delta_y;

    if (!rr_grid_position_is_valid(player->_grid, target_x, target_y))
        return 0;

    targetCellType = rr_grid_get_cell_type(player->_grid, target_x, target_y);

    if (targetCellType == RR_CELL_WALL)
        can_move = 0;

    else if (targetCellType == RR_CELL_BLOCK)
        can_move = rr_player_push(player, target_x, target_y);

    if (can_move){
        rr_grid_set_cell_type(player->_grid, player->pos_x, player->pos_y, RR_CELL_EMPTY);
        rr_player_set_pos(player, target_x, target_y);
    }

    return can_move;
}