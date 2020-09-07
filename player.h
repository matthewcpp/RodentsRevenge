#ifndef RR_PLAYER_H
#define RR_PLAYER_H

#include "entity.h"
#include "point.h"
#include "grid.h"

#include <stdint.h>

typedef struct rrPlayer {
    rrEntity entity;
    rrGrid* _grid;
    int score;
    int lives_remaining;


} rrPlayer;

void rr_player_init(rrPlayer* player, rrGrid* grid);
int rr_player_move(rrPlayer* player, rrPoint* delta);

#endif
