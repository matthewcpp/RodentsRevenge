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
} rrGameState;

typedef struct rrGame {
    rrGrid grid;
    rrPlayer player;
    rrEnemy _enemies[MAX_ENEMIES];
    rrInput* _input;
    char* _current_level;
    int current_round;
    int spawn_count;
    rrGameState state;
} rrGame;

void rr_game_init(rrGame* game, rrInput* input);
void rr_game_uninit(rrGame* game);

void rr_game_update(rrGame* game, int time);

int rr_game_restart(rrGame* game);
int rr_game_set_active_level(rrGame* game, const char* path);

#endif
