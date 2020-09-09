#include "game.h"

#include <string.h>

rrPoint starting_pos = {11,11};

void rr_game_init(rrGame* game, rrInput* input) {
    int i;

    game->_input = input;
    rr_grid_init(&game->grid, RR_GRID_WIDTH, RR_GRID_HEIGHT);
    rr_player_init(&game->player, &game->grid, input);

    for (i = 0; i < MAX_ENEMIES; i++) {
        rr_enemy_init(&game->_enemies[i], &game->player.entity, &game->grid);
    }

    game->_update_time = 0;
    game->_current_level = NULL;
    game->current_round = 0;
}

void rr_game_uninit(rrGame* game) {
    rr_grid_uninit(&game->grid);
}

/* determines the new position for the player */
void rr_game_respawn_player(rrGame* game) {
    /* TODO: pick better respawn location if starting tile is blocked */
    rr_grid_update_entity_position(&game->grid, &game->player.entity, &starting_pos);
    game->player.entity.status = RR_STATUS_ACTIVE;
}

void rr_game_spawn_enemies(rrGame* game) {
    rrPoint pos;
    rr_point_set(&pos, 1, 1);
    rr_point_copy(&game->_enemies[0].entity.position, &pos);
    game->_enemies[0].entity.status = RR_STATUS_ACTIVE;
}

void rr_game_round_clear(rrGame* game) {
    int i;

    for (i = 0; i < MAX_ENEMIES; i++){
        if (game->_enemies[i].entity.status != RR_STATUS_WAITING)
            continue;

        game->_enemies[i].entity.status = RR_STATUS_INACTIVE;
        rr_grid_clear_position(&game->grid, &game->_enemies[i].entity.position);
        rr_grid_create_basic_entity(&game->grid, &game->_enemies[i].entity.position, RR_ENTITY_CHEESE);
    }
}

void rr_game_update(rrGame* game, int time) {
    int i;
    unsigned int round_clear = 1;
    game->_update_time += time;

    rr_player_update(&game->player);

    if (game->player.entity.status == RR_STATUS_KILLED) {
        game->player.lives_remaining -= 1;

        if (game->player.lives_remaining >= 0) {
            rr_game_respawn_player(game);
        }
    }
    else {
        for (i = 0; i < MAX_ENEMIES; i++){
            rr_enemy_update(&game->_enemies[i], time);
            round_clear &= game->_enemies[i].entity.status != RR_STATUS_ACTIVE;
        }

    }

    if (round_clear)
        rr_game_round_clear(game);

    game->_update_time = 0;
}

int rr_game_restart(rrGame* game) {
    if (game->_current_level == NULL)
        return 0;

    rr_grid_clear_position(&game->grid, &starting_pos);
    game->player.lives_remaining = 2;
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

    return rr_grid_load_from_file(&game->grid, game->_current_level);
}
