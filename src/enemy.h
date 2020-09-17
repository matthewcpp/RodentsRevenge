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
int rr_enemy_move(rrEnemy* enemy);

/** Puts enemy in a suspended state so that it will not be reused when spawning additional enemies, but is not active on the board.
 * This state is used when the player collides with an enemy and the death animation is playing.
 */
void rr_enemy_suspend(rrEnemy* enemy);

void* _rr_enemey_create_pooled(void* user_data);

#endif
