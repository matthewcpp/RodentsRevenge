#ifndef RODENTSREVENGE_ENEMY_H
#define RODENTSREVENGE_ENEMY_H

#include "point.h"
#include "entity.h"

typedef struct {
    rrEntity entity;
    int _last_move_time;
    rrEntity* _player;
} rrEnemy;

void rr_enemy_init(rrEnemy* enemy, rrEntity* player, rrGrid* grid);
void rr_enemy_update(rrEnemy* enemy, int time);
#endif
