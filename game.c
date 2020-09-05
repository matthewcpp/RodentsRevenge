#include "game.h"

#include <string.h>

void rr_game_init(rrGame* game) {
    int i;

    rr_grid_init(&game->grid, RR_GRID_WIDTH, RR_GRID_HEIGHT);
    rr_player_init(&game->player, &game->grid);

    for (i = 0; i < MAX_ENEMIES; i++) {
        rr_enemy_init(&game->_enemies[i], &game->player.entity, &game->grid);
    }

    game->_update_time = 0;
    game->_current_level = NULL;
}

void rr_game_uninit(rrGame* game) {
    rr_grid_uninit(&game->grid);
}

/* determines the new position for the player */
void rr_game_respawn_player(rrGame* game) {
    rrPoint pos;
    rr_point_set(&pos, 11, 11);

    /* TODO: pick better respawn location if starting tile is blocked */
    rr_entity_set_pos(&game->player.entity, &pos);
    game->player.entity.status = RR_STATUS_ACTIVE;
}

void rr_game_spawn_enemies(rrGame* game) {
    rrPoint pos;
    rr_point_set(&pos, 1, 1);
    rr_entity_set_pos(&game->_enemies[0].entity, &pos);
    game->_enemies[0].entity.status = RR_STATUS_ACTIVE;
}

void rr_game_update(rrGame* game, int time) {
    int i;
    game->_update_time += time;

    if (game->_update_time <= 64)
        return;

    for (i = 0; i < MAX_ENEMIES; i++)
        rr_enemy_update(&game->_enemies[i], time);

    if (game->player.entity.status == RR_STATUS_KILLED) {
        game->player.lives -= 1;

        if (game->player.lives >= 1) {
            rr_game_respawn_player(game);
        }
    }

    game->_update_time = 0;
}

int rr_game_restart(rrGame* game) {
    if (game->_current_level == NULL)
        return 0;

    game->player.lives = 3;
    rr_game_respawn_player(game);
    rr_game_spawn_enemies(game);

    return 1;
}

int rr_game_set_active_level(rrGame* game, const char* path){
    size_t len;
    if (game->_current_level) {
        free(game->_current_level);
    }

    len = strlen(path);
    game->_current_level = malloc(len + 1);
    strcpy(game->_current_level, path);

    return rr_grid_load_from_file(&game->grid, path);
}
