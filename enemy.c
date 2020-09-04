#include "enemy.h"

void rr_enemy_init(rrEnemy* enemy, rrEntity* player, rrGrid* grid) {
    rr_entity_init(&enemy->entity, RR_ENTITY_ENEMY, enemy, grid);
    enemy->_player = player;
}

int rr_enemy_can_move(rrEnemy* enemy) {
    int i;
    rrPoint testPoint;

    static rrPoint deltas[8] = {
            {-1, -1}, {0, -1}, {1, -1},
            {-1, 0}, {1, 0},
            {-1, 1}, {0, 1}, {1, 1}};

    for (i = 0; i < 8; i++) {
        rr_point_add(&testPoint, &enemy->entity.position, &deltas[i]);

        if (!rr_grid_cell_is_blocked(enemy->entity._grid, &testPoint))
            return 1;
    }

    return 0;
}

void rr_enemy_update(rrEnemy* enemy) {
    if (enemy->entity.status == RR_STATUS_INACTIVE)
        return;

    if (rr_enemy_can_move(enemy))
        enemy->entity.status = RR_STATUS_ACTIVE;
    else
        enemy->entity.status = RR_STATUS_WAITING;
}
