#ifndef RR_GAME_H
#define RR_GAME_H

#include "clock.h"
#include "grid.h"
#include "player.h"
#include "enemy.h"
#include "input.h"

#define RR_GAME_MAX_ENEMIES 9

typedef enum {
    RR_GAME_STATE_UNSTARTED,
    RR_GAME_STATE_PLAYING,
    RR_GAME_STATE_WINDING_CLOCK,
    RR_GAME_STATE_PAUSED
} rrGameState;

typedef struct rrGame {
    rrGrid* grid;
    rrPlayer player;
    rrClock clock;
    rrEnemy _enemies[RR_GAME_MAX_ENEMIES];
    rrInput* _input;
    int current_level;
    int spawn_count;
    int enemy_index;
    int enemy_count;
    rrGameState state;
    char* _asset_path;
    size_t _asset_path_len;
} rrGame;

rrGame* rr_game_create(rrInput* input, const char* asset_path);
void rr_game_destroy(rrGame* game);

void rr_game_update(rrGame* game, int time);

int rr_game_restart(rrGame* game);
int rr_game_set_active_level(rrGame* game, int level_num);

/** Sets the number of enemies the player must defeat before they can advance to the next level. */
void rr_game_set_enemy_count(rrGame* game, int enemy_count);

#endif
