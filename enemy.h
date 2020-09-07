#ifndef RODENTSREVENGE_ENEMY_H
#define RODENTSREVENGE_ENEMY_H

#include "entity.h"
#include "point.h"
#include "grid.h"

typedef struct {
    rrEntity entity;
    rrGrid* _grid;
    int _last_move_time;
    rrEntity* _player;
} rrEnemy;

void rr_enemy_init(rrEnemy* enemy, rrEntity* player, rrGrid* grid);
void rr_enemy_update(rrEnemy* enemy, int time);
void rr_enemy_move(rrEnemy* enemy);
#endif
