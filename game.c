#include "game.h"

void rr_game_init(rrGame* game) {
    rr_grid_init(&game->grid, RR_GRID_WIDTH, RR_GRID_HEIGHT);
    rr_player_init(&game->player, &game->grid);
}

void rr_game_uninit(rrGame* game) {
    rr_grid_uninit(&game->grid);
}