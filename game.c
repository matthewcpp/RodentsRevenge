#include "game.h"

void rr_game_init(rrGame* game) {
    rr_grid_init(&game->grid, RR_GRID_WIDTH, RR_GRID_HEIGHT);
    rr_player_init(&game->player, &game->grid);
}

void rr_game_uninit(rrGame* game) {
    rr_grid_uninit(&game->grid);
}

int rr_game_new_level(rrGame* game, const char* path){
    if (!rr_grid_load_from_file(&game->grid, path))
        return 0;

    rr_player_set_pos(&game->player, 11, 11);

    return 1;
}