#ifndef RR_GAME_H
#define RR_GAME_H

#include "clock.h"
#include "grid.h"
#include "player.h"
#include "enemy.h"
#include "input.h"

#define MAX_ENEMIES 8

typedef enum {
    RR_GAME_STATE_UNSTARTED,
    RR_GAME_STATE_PLAYING,
    RR_GAME_STATE_PAUSED
} rrGameState;

typedef struct rrGame {
    rrGrid* grid;
    rrPlayer player;
    rrClock* clock;
    rrEnemy _enemies[MAX_ENEMIES];
    rrInput* _input;
    int current_level;
    int current_round;
    int rounds_per_level;
    int spawn_count;
    rrGameState state;
    char* _asset_path;
    size_t _asset_path_len;
} rrGame;

rrGame* rr_game_create(rrInput* input, const char* asset_path);
void rr_game_destroy(rrGame* game);

void rr_game_update(rrGame* game, int time);

int rr_game_restart(rrGame* game);
int rr_game_set_active_level(rrGame* game, int level_num);
void rr_game_set_rounds_per_level(rrGame* game, int rounds_per_level);

#endif
