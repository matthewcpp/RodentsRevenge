#ifndef RR_PLAYER_H
#define RR_PLAYER_H

#include "entity.h"
#include "point.h"
#include "grid.h"
#include "input.h"

#include <stdint.h>

typedef struct rrPlayer {
    rrEntity entity;
    rrGrid* _grid;
    rrInput* _input;
    int score;
    int lives_remaining;
} rrPlayer;

void rr_player_init(rrPlayer* player, rrGrid* grid, rrInput* input);
void rr_player_update(rrPlayer* player);
void rr_player_move(rrPlayer* player, rrPoint* delta);
void rr_player_kill(rrPlayer* player);

#endif
