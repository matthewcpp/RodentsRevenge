#include "game.h"
#include "spawn.h"

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

    game->_current_level = NULL;
    game->current_round = 0;
    game->spawn_count = 1;
}

void rr_game_uninit(rrGame* game) {
    rr_grid_uninit(&game->grid);
}

/* TODO: investigate player respawn behavior further.  Perhaps based on distance to enemy? */
void rr_game_respawn_player(rrGame* game) {
    rrEntity* entity = rr_grid_get_cell(&game->grid, &starting_pos);
    rrPoint spawn_pos;
    rr_point_copy(&spawn_pos, &starting_pos);

    if (entity && !rr_entity_is_static(entity))
        rr_get_spawn_pos(&game->grid, &spawn_pos);

    rr_grid_update_entity_position(&game->grid, &game->player.entity, &spawn_pos);
    game->player.entity.status = RR_STATUS_ACTIVE;
}

int rr_game_get_next_inactive_enemy_index(rrGame* game) {
    int i;

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (game->_enemies[i].entity.status == RR_STATUS_INACTIVE)
            return i;
    }

    return MAX_ENEMIES;
}

void rr_game_spawn_enemies(rrGame* game) {
    int i;
    for (i = 0; i < game->spawn_count; i++) {
        int enemy_index = rr_game_get_next_inactive_enemy_index(game);
        rrPoint pos;

        if (enemy_index == MAX_ENEMIES)
            break;

        rr_get_spawn_pos(&game->grid, &pos);
        rr_grid_update_entity_position(&game->grid, &game->_enemies[enemy_index].entity, &pos);
        game->_enemies[enemy_index].entity.status = RR_STATUS_ACTIVE;
    }

    game->spawn_count = (game->spawn_count == 1) ? 2 : 1;
}

void rr_game_round_clear(rrGame* game) {
    if (game->current_round <= 3) {
        int i;

        for (i = 0; i < MAX_ENEMIES; i++){
            if (game->_enemies[i].entity.status != RR_STATUS_WAITING)
                continue;

            rr_grid_clear_position(&game->grid, &game->_enemies[i].entity.position);
            rr_grid_create_basic_entity(&game->grid, &game->_enemies[i].entity.position, RR_ENTITY_CHEESE);
            game->_enemies[i].entity.status = RR_STATUS_INACTIVE;
            rr_point_set(&game->_enemies[i].entity.position, -1, -1);
        }

        rr_game_spawn_enemies(game);
        game->current_round += 1;
    }
}

void rr_game_update(rrGame* game, int time) {
    rr_player_update(&game->player);

    if (game->player.entity.status == RR_STATUS_KILLED) {
        game->player.lives_remaining -= 1;

        if (game->player.lives_remaining >= 0) {
            rr_game_respawn_player(game);
        }
    }
    else {
        int i;
        unsigned int round_clear = 1;

        for (i = 0; i < MAX_ENEMIES; i++){
            rr_enemy_update(&game->_enemies[i], time);
            round_clear &= game->_enemies[i].entity.status != RR_STATUS_ACTIVE;
        }

        if (round_clear)
            rr_game_round_clear(game);
    }
}

int rr_game_restart(rrGame* game) {
    if (game->_current_level == NULL)
        return 0;

    rr_grid_clear_position(&game->grid, &starting_pos);
    game->player.lives_remaining = 2;
    game->current_round = 1;
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
