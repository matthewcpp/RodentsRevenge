#ifndef RR_PLAYER_H
#define RR_PLAYER_H

#include "entity.h"
#include "point.h"

#include <stdint.h>

typedef struct rrPlayer {
    int score;
    int lives;
    rrEntity entity;

} rrPlayer;

void rr_player_init(rrPlayer* player, rrGrid* grid);
int rr_player_move(rrPlayer* player, rrPoint* delta);

#endif
