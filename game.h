#ifndef RR_GAME_H
#define RR_GAME_H

#include "grid.h"
#include "player.h"
#include "enemy.h"

#define MAX_ENEMIES 8

typedef struct rrGame {
    rrGrid grid;
    rrPlayer player;
    rrEnemy _enemies[MAX_ENEMIES];
    int _update_time;
    char* _current_level;
} rrGame;

void rr_game_init(rrGame* game);
void rr_game_uninit(rrGame* game);

void rr_game_update(rrGame* game, int time);

int rr_game_restart(rrGame* game);
int rr_game_set_active_level(rrGame* game, const char* path);

#endif
