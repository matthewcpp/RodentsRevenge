#include "enemy.h"

#include <limits.h>

void rr_enemy_init(rrEnemy* enemy, rrEntity* player, rrGrid* grid) {
    rr_entity_init(&enemy->entity, RR_ENTITY_ENEMY, enemy, grid);
    enemy->_player = player;
    enemy->_last_move_time = 0;
}

static rrPoint deltas[8] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1, 0}, {1, 0},
        {-1, 1}, {0, 1}, {1, 1}};

int rr_enemy_can_move(rrEnemy* enemy) {
    int i;
    rrPoint testPoint;

    for (i = 0; i < 8; i++) {
        rr_point_add(&testPoint, &enemy->entity.position, &deltas[i]);

        if (!rr_grid_cell_is_blocked(enemy->entity._grid, &testPoint))
            return 1;
    }

    return 0;
}

void rr_enemy_move(rrEnemy* enemy) {
    int i;
    int shortest_dist = INT_MAX;
    rrPoint move_pos, target_point;

    for (i = 0; i < 8; i++) {
        int dist_to_player;
        rr_point_add(&target_point, &enemy->entity.position, &deltas[i]);

        if (rr_grid_cell_is_blocked(enemy->entity._grid, &target_point))
            continue;

        dist_to_player = rr_point_dist_squared(&target_point, &enemy->_player->position);
        if (dist_to_player < shortest_dist) {
            rr_point_copy(&move_pos, &target_point);
            shortest_dist = dist_to_player;
        }
    }

    if (shortest_dist != INT_MAX) {
        enemy->entity.status = RR_STATUS_ACTIVE;
        rr_entity_set_pos(&enemy->entity, &move_pos);
    }
    else { /* Enemy was not able to move and is pinned*/
        enemy->entity.status = RR_STATUS_WAITING;
    }
}

void rr_enemy_update(rrEnemy* enemy, int time) {

    if (enemy->entity.status == RR_STATUS_INACTIVE)
        return;

    enemy->_last_move_time += time;
    if (enemy->_last_move_time >= 1000) {
        rr_enemy_move(enemy);
        enemy->_last_move_time = 0;
    }

}
