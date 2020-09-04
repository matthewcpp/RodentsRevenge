#include "game.h"

void rr_game_init(rrGame* game) {
    int i;

    rr_grid_init(&game->grid, RR_GRID_WIDTH, RR_GRID_HEIGHT);
    rr_player_init(&game->player, &game->grid);

    for (i = 0; i < MAX_ENEMIES; i++) {
        rr_enemy_init(&game->_enemies[i], &game->player.entity, &game->grid);
    }

    game->_update_time = 0;
}

void rr_game_uninit(rrGame* game) {
    rr_grid_uninit(&game->grid);
}

void rr_game_update(rrGame* game, int time) {
    int i;
    game->_update_time += time;

    if (game->_update_time <= 64)
        return;

    for (i = 0; i < MAX_ENEMIES; i++)
        rr_enemy_update(&game->_enemies[i]);

    game->_update_time = 0;
}

int rr_game_new_level(rrGame* game, const char* path){
    rrPoint pos;
    if (!rr_grid_load_from_file(&game->grid, path))
        return 0;

    rr_point_set(&pos, 11, 11);
    rr_entity_set_pos(&game->player.entity, &pos);
    game->player.entity.status = RR_STATUS_ACTIVE;

    rr_point_set(&pos, 1, 1);
    rr_entity_set_pos(&game->_enemies[0].entity, &pos);
    game->_enemies[0].entity.status = RR_STATUS_ACTIVE;

    return 1;
}
