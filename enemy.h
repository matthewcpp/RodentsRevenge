#ifndef RODENTSREVENGE_ENEMY_H
#define RODENTSREVENGE_ENEMY_H

#include "point.h"
#include "entity.h"

typedef struct {
    rrPoint position;
    rrEntity entity;

    rrEntity* _player;
} rrEnemy;

void rr_enemy_init(rrEnemy* enemy, rrEntity* player, rrGrid* grid);

#endif
