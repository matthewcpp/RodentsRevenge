#ifndef RR_PLAYER_H
#define RR_PLAYER_H

#include "grid.h"

#include <stdint.h>

typedef struct rrPlayer {
    int score;
    int pos_x;
    int pos_y;
    int lives;

    rrGrid* _grid;
} rrPlayer;

void rr_player_init(rrPlayer* player, rrGrid* grid);
int rr_player_set_pos(rrPlayer* player, int x, int y);
int rr_player_move(rrPlayer* player, int delta_x, int delta_y);

#endif
