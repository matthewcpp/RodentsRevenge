#ifndef RR_PLAYER_H
#define RR_PLAYER_H

#include "grid.h"
#include "point.h"

#include <stdint.h>

typedef struct rrPlayer {
    int score;
    rrPoint position;
    int lives;

    rrGrid* _grid;
} rrPlayer;

void rr_player_init(rrPlayer* player, rrGrid* grid);
int rr_player_set_pos(rrPlayer* player, rrPoint* position);
int rr_player_move(rrPlayer* player, rrPoint* delta);

#endif
