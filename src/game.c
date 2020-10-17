#include "game.h"
#include "game_private.h"

#include "debug.h"
#include "util.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

rrPoint starting_pos = {11,11};

#define RR_LEVEL_COUNT 4
#define RR_CLOCK_WIND_SPEED_MOD 100

#define RR_GAME_DEFAULT_ENEMY_UPDATE_FREQUENCY 1000

rrGame* rr_game_create(rrInput* input, rrRenderer* renderer, const char* asset_path) {
    rrGame* game = malloc(sizeof(rrGame));

    game->_input = input;
    game->_renderer = renderer;
    game->grid = rr_grid_create(RR_GRID_WIDTH, RR_GRID_HEIGHT);
    rr_player_init(&game->player, game->grid, input, renderer);

    game->_enemies = cutil_vector_create(cutil_trait_ptr());
    game->_yarns = cutil_vector_create(cutil_trait_ptr());
    game->_enemy_pool = rr_pool_create(_rr_enemy_create_pooled, _rr_enemy_reset_pooled, rr_pool_default_delete_func, game);
    game->_yarn_pool = rr_pool_create(_rr_yarn_create_pooled, _rr_yarn_reset_pooled, rr_pool_default_delete_func, game);
    game->_spawner = rr_spawner_create(game->grid, game->_enemy_pool, game->_yarn_pool);

    game->state = RR_GAME_STATE_UNSTARTED;
    rr_clock_init(&game->clock);
    game->clock.target_pos = 5;

    game->_asset_path_len = strlen(asset_path);
    game->_asset_path = malloc(game->_asset_path_len + 1);
    strcpy(game->_asset_path, asset_path);

    game->enemy_update_time = 0;
    game->enemy_update_frequency = RR_GAME_DEFAULT_ENEMY_UPDATE_FREQUENCY;

    game->yarn_spawn_time_min = 0;
    game->yarn_spawn_time_max = 0;
    game->yarn_spawn_time = 0;

    rr_game_set_active_level(game, 1);

    return game;
}

void rr_game_destroy(rrGame* game) {
    rr_pool_return_vec(game->_enemy_pool, game->_enemies);
    rr_pool_return_vec(game->_yarn_pool, game->_yarns);
    cutil_vector_destroy(game->_enemies);
    cutil_vector_destroy(game->_yarns);
    rr_pool_destroy(game->_enemy_pool);
    rr_pool_destroy(game->_yarn_pool);

    rr_grid_destroy(game->grid);
    rr_spawner_destroy(game->_spawner);

    free(game->_asset_path);

    free(game);
}

rrClock* rr_game_get_clock(rrGame* game) {
    return &game->clock;
}
rrPlayer* rr_game_get_player(rrGame* game) {
    return &game->player;
}

rrGrid* rr_game_get_grid(rrGame* game) {
    return game->grid;
}

int rr_yarn_enabled(rrGame* game) {
    return game->yarn_spawn_time_min > 0 && game->yarn_spawn_time_max > 0;
}

/* TODO: investigate player respawn behavior further.  Perhaps based on distance to enemy? */
void rr_game_respawn_player(rrGame* game) {
    rrEntity* entity = rr_grid_get_entity_at_position(game->grid, &starting_pos);
    rrPoint spawn_pos;
    rr_point_copy(&spawn_pos, &starting_pos);

    if (entity){
        if (rr_entity_is_static(entity))
            rr_grid_destroy_basic_entity(game->grid, entity);
        else
            rr_spawner_get_spawn_pos(game->_spawner, &spawn_pos);
    }

    rr_entity_place_in_grid_cell(&game->player.entity, game->grid, &spawn_pos);
    game->player.entity.status = RR_STATUS_ACTIVE;
}

void rr_game_round_clear(rrGame* game) {
    int i;
    int count = (int)cutil_vector_size(game->_enemies);

    for (i = 0; i < count; i++){
        rrEnemy* enemy;
        rrPoint position;
        cutil_vector_get(game->_enemies, i, &enemy);

        assert (enemy->entity.status == RR_STATUS_WAITING);

        rr_point_copy(&position, &enemy->entity.position);
        rr_entity_remove_from_grid(&enemy->entity, enemy->_grid);
        rr_pool_return(game->_enemy_pool, enemy);

        rr_grid_create_basic_entity(game->grid, &position, RR_ENTITY_CHEESE);

        game->player.score += 1;
    }

    cutil_vector_clear(game->_enemies);
}

void rr_game_over(rrGame* game) {
    game->state = RR_GAME_STATE_OVER;
}

void rr_game_reset(rrGame* game) {
    size_t i;

    if (!rr_entity_position_is_invalid(&game->player.entity))
        rr_entity_remove_from_grid(&game->player.entity, game->grid);

    for (i = 0; i < cutil_vector_size(game->_enemies); i++) {
        rrEnemy* enemy;
        cutil_vector_get(game->_enemies, i, &enemy);
        rr_entity_remove_from_grid(&enemy->entity, game->grid);
        rr_pool_return(game->_enemy_pool, enemy);
    }
    cutil_vector_clear(game->_enemies);

    for (i = 0; i < cutil_vector_size(game->_yarns); i++) {
        rrYarn* yarn;
        cutil_vector_get(game->_yarns, i, &yarn);
        rr_entity_remove_from_grid(&yarn->entity, game->grid);
        rr_pool_return(game->_yarn_pool, yarn);
    }
    cutil_vector_clear(game->_yarns);

    rr_game_set_active_level(game, game->current_level);
    game->state = RR_GAME_STATE_UNSTARTED;
    game->enemy_update_time = 0;
    game->yarn_spawn_time = 0;
}

void rr_game_set_next_yarn_spawn_time(rrGame* game) {
    if (rr_yarn_enabled(game))
        game->yarn_spawn_time = rand_between(game->yarn_spawn_time_min, game->yarn_spawn_time_max);
    else
        game->yarn_spawn_time = 0;
}

void rr_game_next_level(rrGame* game){
    int next_level = game->current_level < RR_LEVEL_COUNT ? game->current_level + 1 : 1;

    rr_entity_remove_from_grid(&game->player.entity, game->grid);
    rr_pool_return_vec(game->_yarn_pool, game->_yarns);
    cutil_vector_clear(game->_yarns);

    assert(cutil_vector_empty(game->_enemies));

    rr_game_set_active_level(game, next_level);
    rr_game_respawn_player(game);
    game->enemy_update_time = 0;
    rr_spawner_spawn_enemies(game->_spawner, game->_enemies);

    rr_game_set_next_yarn_spawn_time(game);
}

void rr_game_update_yarn(rrGame* game, int time) {
    int i, count = (int)cutil_vector_size(game->_yarns);

    for (i = count - 1; i >= 0; i--) {
        rrYarn* yarn;
        cutil_vector_get(game->_yarns, i, &yarn);
        rr_yarn_update(yarn, time);

        if (yarn->entity.status == RR_STATUS_KILLED) {
            cutil_vector_remove(game->_yarns, i);
            rr_pool_return(game->_yarn_pool, yarn);
        }
    }
}

void rr_game_update_yarn_spawn_timer(rrGame* game, int time) {
    if (!rr_yarn_enabled(game) || cutil_vector_size(game->_yarns) >= 2)
        return;

    game->yarn_spawn_time -= time;

    if (game->yarn_spawn_time < 0) {
        rr_spawner_spawn_yarn(game->_spawner, game->_yarns);
        rr_game_set_next_yarn_spawn_time(game);
    }
}

int rr_game_update_enemies(rrGame* game, int time) {
    int i, count, round_clear = 0;

    game->enemy_update_time += time;
    if (game->enemy_update_time < game->enemy_update_frequency)
        return round_clear;

    count = (int)cutil_vector_size(game->_enemies);
    round_clear = 1;

    for (i = 0; i < count; i++){
        rrEnemy* enemy;
        cutil_vector_get(game->_enemies, i, &enemy);
        rr_enemy_update(enemy);

        /* check to see if any previously spawned enemies are still active. */
        if (enemy->entity.status == RR_STATUS_ACTIVE || enemy->entity.status == RR_STATUS_SUSPENDED)
            round_clear = 0;
    }

    game->enemy_update_time -= game->enemy_update_frequency;

    return round_clear;
}

void rr_game_update_player_active(rrGame* game, int time) {
    unsigned int round_clear = rr_game_update_enemies(game, time);
    rr_game_update_yarn_spawn_timer(game, time);
    rr_game_update_yarn(game, time);

    if (rr_clock_did_tick_seconds(&game->clock)) {
        if (game->clock.seconds_pos == 14 || game->clock.seconds_pos == 0)
        game->player.score += 1;
    }

    /* Player has not defeated all enemies in time...more will spawn now! */
    if (rr_clock_did_tick_target(&game->clock) && game->clock.target_pos != 0) {
        rr_clock_advance_target(&game->clock);
        rr_spawner_spawn_enemies(game->_spawner, game->_enemies);
    }

    if (round_clear) {
        rr_game_round_clear(game);

        if (game->clock.target_pos != 0)
            game->state = RR_GAME_STATE_WINDING_CLOCK;
        else
            rr_game_next_level(game);
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

void rr_game_update_state_winding_clock(rrGame* game, int time) {
    rr_clock_update(&game->clock, time * RR_CLOCK_WIND_SPEED_MOD);

    if (rr_clock_did_tick_target(&game->clock)) {
        game->clock.update_time = 0;

        rr_clock_advance_target(&game->clock);

        rr_spawner_spawn_enemies(game->_spawner, game->_enemies);
        game->state = RR_GAME_STATE_PLAYING;
    }
}

void rr_game_update_state_playing(rrGame* game, int time) {
    rr_player_update(&game->player, time);
    rr_clock_update(&game->clock, time);

    if (game->player.entity.status == RR_STATUS_KILLED)
        rr_game_update_player_killed(game);
    else if (game->player.entity.status == RR_STATUS_ACTIVE || game->player.entity.status == RR_STATUS_STUCK)
        rr_game_update_player_active(game, time);
}

void rr_game_update_state_unstarted(rrGame* game) {
    if (rr_input_button_down(game->_input, RR_INPUT_BUTTON_START))
        rr_game_restart(game);
}

void rr_game_set_paused(rrGame* game, int paused) {
    if (paused)
        game->state = RR_GAME_STATE_PAUSED;
    else
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

        case RR_GAME_STATE_WINDING_CLOCK:
            rr_game_update_state_winding_clock(game, time);
            break;

        case RR_GAME_STATE_PAUSED:
        case RR_GAME_STATE_OVER:
            break;
    }
}

int rr_game_restart(rrGame* game) {
    assert(game->state == RR_GAME_STATE_UNSTARTED || game->state == RR_GAME_STATE_OVER);

    if (game->state == RR_GAME_STATE_OVER)
        rr_game_reset(game);

    game->player.score = 0;
    game->player.lives_remaining = 2;
    game->player.time_stuck = 0;

    rr_clock_reset(&game->clock);

    game->state = RR_GAME_STATE_PLAYING;
    rr_game_respawn_player(game);
    rr_spawner_spawn_enemies(game->_spawner, game->_enemies);
    rr_game_set_next_yarn_spawn_time(game);

    return 1;
}

int rr_game_load_level(rrGame* game, const char* path) {
    int file_loaded = rr_grid_load_from_file(game->grid, path);

    rr_spawner_set_properties(game->_spawner, rr_grid_get_properties(game->grid));
    rr_clock_set_properties(&game->clock, rr_grid_get_properties(game->grid));
    rr_debug_load_properties(game);

    return file_loaded;
}

int rr_game_set_active_level(rrGame* game, int level_num){
    int file_loaded;

    char* path_buffer = malloc(game->_asset_path_len + 32);
    sprintf(path_buffer, "%s%slevels%slevel%02d.txt", game->_asset_path, rr_path_sep(), rr_path_sep(), level_num);
    file_loaded = rr_game_load_level(game, path_buffer);

    free (path_buffer);
    game->current_level = level_num;

    return file_loaded;
}

int rr_game_load_debug_level(rrGame* game, const char* path){
    int file_loaded = rr_game_load_level(game, path);
    game->current_level = 0;
    return file_loaded;
}

rrGameState rr_game_get_state(rrGame* game) {
    return game->state;
}

int rr_game_get_level_count() {
    return RR_LEVEL_COUNT;
}

rrRenderer* rr_game_get_renderer(rrGame* game) {
    return game->_renderer;
}
