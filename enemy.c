#include "enemy.h"

void rr_enemy_init(rrEnemy* enemy, rrEntity* player, rrGrid* grid) {
    rr_entity_init(&enemy->entity, RR_CELL_CAT, enemy, grid);
    enemy->_player = player;
}
