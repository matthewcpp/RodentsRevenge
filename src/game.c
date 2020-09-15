#include "game.h"

#include "spawn.h"
#include "util.h"

#include <string.h>
#include <stdio.h>

rrPoint starting_pos = {11,11};

#define RR_LEVEL_COUNT 2
#define RR_DEFAULT_ROUNDS_PER_LEVEL 6

rrGame* rr_game_create(rrInput* input, const char* asset_path) {
    int i;
    rrGame* game = malloc(sizeof(rrGame));

    game->_input = input;
    game->grid = rr_grid_create(RR_GRID_WIDTH, RR_GRID_HEIGHT);
    rr_player_init(&game->player, game->grid, input);

    for (i = 0; i < MAX_ENEMIES; i++) {
        rr_enemy_init(&game->_enemies[i], &game->player.entity, game->grid);
    }

    game->current_round = 1;
    game->rounds_per_level = RR_DEFAULT_ROUNDS_PER_LEVEL;
    game->spawn_count = 1;
    game->state = RR_GAME_STATE_UNSTARTED;
    rr_clock_init(&game->clock);
    game->clock.target_pos = 5;

    game->_asset_path_len = strlen(asset_path);
    game->_asset_path = malloc(game->_asset_path_len + 1);
    strcpy(game->_asset_path, asset_path);

    rr_game_set_active_level(game, 1);

    return game;
}

void rr_game_destroy(rrGame* game) {
    rr_grid_destroy(game->grid);
    free(game);
}

/* TODO: investigate player respawn behavior further.  Perhaps based on distance to enemy? */
void rr_game_respawn_player(rrGame* game) {
    rrEntity* entity = rr_grid_get_entity_at_position(game->grid, &starting_pos);
    rrPoint spawn_pos;
    rr_point_copy(&spawn_pos, &starting_pos);

    if (entity){
        if (rr_entity_is_static(entity))
            rr_grid_clear_position(game->grid, &starting_pos);
        else
            rr_get_spawn_pos(game->grid, &spawn_pos);
    }

    rr_entity_place_in_grid_cell(&game->player.entity, game->grid, &spawn_pos);
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

        rr_get_spawn_pos(game->grid, &pos);
        rr_entity_place_in_grid_cell(&game->_enemies[enemy_index].entity, game->grid, &pos);
        game->_enemies[enemy_index].entity.status = RR_STATUS_ACTIVE;
    }

    game->spawn_count = (game->spawn_count == 1) ? 2 : 1;
}

void rr_game_round_clear(rrGame* game) {
    if (game->current_round <= game->rounds_per_level) {
        int i;

        for (i = 0; i < MAX_ENEMIES; i++){
            rrEnemy* enemy = &game->_enemies[i];
            rrPoint position;

            if (enemy->entity.status != RR_STATUS_WAITING)
                continue;

            rr_point_copy(&position, &enemy->entity.position);
            rr_entity_remove_from_grid(&enemy->entity, enemy->_grid);
            game->_enemies[i].entity.status = RR_STATUS_INACTIVE;

            rr_grid_create_basic_entity(game->grid, &position, RR_ENTITY_CHEESE);

            game->player.score += 1;
        }
    }
}

void rr_game_over(rrGame* game) {
    int i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        rr_entity_deactivate(&game->_enemies[i].entity);
    }

    rr_entity_deactivate(&game->player.entity);

    /* TODO: Preserve last board state while game over state active */
    rr_game_set_active_level(game, game->current_level);
    game->state = RR_GAME_STATE_UNSTARTED;
}

void rr_game_next_level(rrGame* game){
    int next_level = game->current_level < RR_LEVEL_COUNT ? game->current_level + 1 : 1;
    rr_entity_remove_from_grid(&game->player.entity, game->grid);
    rr_game_set_active_level(game, next_level);
    rr_game_respawn_player(game);
    rr_game_spawn_enemies(game);

    game->current_round = 1;
}

void rr_game_update_player_active(rrGame* game, int time) {
    int i;
    unsigned int round_clear = 1;

    if (rr_clock_did_tick_seconds(&game->clock)) {
        if (game->clock.seconds_pos == 14 || game->clock.seconds_pos == 0)
        game->player.score += 1;
    }

    for (i = 0; i < MAX_ENEMIES; i++){
        rrEnemy* enemy = &game->_enemies[i];
        rr_enemy_update(enemy, time);
        if (enemy->entity.status == RR_STATUS_ACTIVE || enemy->entity.status == RR_STATUS_SUSPENDED)
            round_clear = 0;
    }

    if (round_clear) {
        rr_game_round_clear(game);

        if (game->current_round < game->rounds_per_level) {
            rr_game_spawn_enemies(game);
            game->current_round += 1;
        }
        else {
            rr_game_next_level(game);
        }
    }
}

void rr_game_update_player_killed(rrGame* game) {
    game->player.lives_remaining -= 1;

    if (game->player.lives_remaining >= 0)
        rr_game_respawn_player(game);
    else {
        rr_game_over(game);
    }
}

void rr_game_update_state_playing(rrGame* game, int time) {
    if (rr_input_button_down(game->_input, RR_INPUT_BUTTON_START)) {
        game->state = RR_GAME_STATE_PAUSED;
        return;
    }

    rr_player_update(&game->player, time);
    rr_clock_update(&game->clock, time);

    if (game->player.entity.status == RR_STATUS_KILLED)
        rr_game_update_player_killed(game);
    else if (game->player.entity.status == RR_STATUS_ACTIVE)
        rr_game_update_player_active(game, time);
}

void rr_game_update_state_unstarted(rrGame* game) {
    if (rr_input_button_down(game->_input, RR_INPUT_BUTTON_START))
        rr_game_restart(game);
}

void rr_game_update_state_paused(rrGame* game) {
    if (rr_input_button_down(game->_input, RR_INPUT_BUTTON_START))
        game->state = RR_GAME_STATE_PLAYING;
}

void rr_game_update(rrGame* game, int time) {
    switch (game->state) {
        case RR_GAME_STATE_UNSTARTED:
            rr_game_update_state_unstarted(game);
            break;

        case RR_GAME_STATE_PLAYING:
            rr_game_update_state_playing(game, time);
            break;

        case RR_GAME_STATE_PAUSED:
            rr_game_update_state_paused(game);
            break;
    }
}

int rr_game_restart(rrGame* game) {
    game->player.score = 0;
    game->player.lives_remaining = 2;
    game->current_round = 1;

    rr_clock_reset(&game->clock);
    game->clock.target_pos = 5;

    game->state = RR_GAME_STATE_PLAYING;
    rr_game_respawn_player(game);
    rr_game_spawn_enemies(game);

    return 1;
}

int rr_game_set_active_level(rrGame* game, int level_num){
    int file_loaded;

    char* path_buffer = malloc(game->_asset_path_len + 32);
    sprintf(path_buffer, "%s%slevels%slevel%02d.txt", game->_asset_path, rr_path_sep(), rr_path_sep(), level_num);
    file_loaded = rr_grid_load_from_file(game->grid, path_buffer);
    free (path_buffer);
    game->current_level = level_num;

    return file_loaded;
}

void rr_game_set_rounds_per_level(rrGame* game, int rounds_per_level) {
    game->rounds_per_level = rounds_per_level;
}
