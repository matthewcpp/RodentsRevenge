#ifndef RR_GAME_H
#define RR_GAME_H

#include "player.h"
#include "grid.h"

typedef struct rrGame {
    rrGrid grid;
    rrPlayer player;
} rrGame;

void rr_game_init(rrGame* game);
void rr_game_uninit(rrGame* game);

#endif
