#ifndef RR_GAME_H
#define RR_GAME_H

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
    rrGrid grid;
    rrPlayer player;
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

void rr_game_init(rrGame* game, rrInput* input, const char* asset_path);
void rr_game_uninit(rrGame* game);

void rr_game_update(rrGame* game, int time);

int rr_game_restart(rrGame* game);
int rr_game_set_active_level(rrGame* game, int level_num);
void rr_game_set_rounds_per_level(rrGame* game, int rounds_per_level);

#endif
