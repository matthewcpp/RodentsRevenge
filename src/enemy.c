#include "enemy.h"

#include "game.h"

#include <limits.h>

void rr_enemy_init(rrEnemy* enemy, rrEntity* player, rrGrid* grid) {
    rr_entity_init(&enemy->entity, RR_ENTITY_ENEMY);
    enemy->_grid = grid;
    enemy->_player = player;
    enemy->_last_move_time = 0;
}

static rrPoint deltas[8] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1, 0}, {1, 0},
        {-1, 1}, {0, 1}, {1, 1}};

int rr_enemy_move(rrEnemy* enemy) {
    int i;
    int shortest_dist = INT_MAX;
    rrPoint move_pos, target_point;

    for (i = 0; i < 8; i++) {
        int dist_to_player;
        rr_point_add(&target_point, &enemy->entity.position, &deltas[i]);

        if (rr_point_equals(&target_point, &enemy->_player->position)) {
            if (enemy->_player->status == RR_STATUS_ACTIVE || enemy->_player->status == RR_STATUS_STUCK) {
                rr_enemy_suspend(enemy);
                rr_player_kill((rrPlayer*)enemy->_player, &enemy->entity);
                return 1;
            }
            else{
                return 0;
            }

        }

        if (rr_grid_get_entity_at_position(enemy->_grid, &target_point) != NULL)
            continue;

        dist_to_player = rr_point_dist_squared(&target_point, &enemy->_player->position);
        if (dist_to_player < shortest_dist) {
            rr_point_copy(&move_pos, &target_point);
            shortest_dist = dist_to_player;
        }
    }

    /* Enemy was not able to move and is pinned*/
    if (shortest_dist == INT_MAX) {
        enemy->entity.status = RR_STATUS_WAITING;
        return 0;
    }
    else {
        enemy->entity.status = RR_STATUS_ACTIVE;
        rr_entity_move_to_grid_cell(&enemy->entity, enemy->_grid, &move_pos);
        return 1;
    }
}

void rr_enemy_update(rrEnemy* enemy, int time) {
    if (enemy->entity.status == RR_STATUS_ACTIVE || enemy->entity.status == RR_STATUS_WAITING) {
        enemy->_last_move_time += time;
        if (enemy->_last_move_time >= 1000) {
            rr_enemy_move(enemy);
            enemy->_last_move_time = 0;
        }
    }
}

void rr_enemy_suspend(rrEnemy* enemy) {
    rr_entity_remove_from_grid(&enemy->entity, enemy->_grid);
    enemy->entity.status = RR_STATUS_SUSPENDED;
}

void* _rr_enemy_create_pooled(void* user_data) {
    rrGame* game = (rrGame*)user_data;

    rrEnemy* enemy = malloc(sizeof(rrEnemy));
    rr_enemy_init(enemy, &rr_game_get_player(game)->entity, rr_game_get_grid(game));

    return enemy;
}

/** ensures that the enemy has been removed from the board before it is returned to the reserve.
 * Failure to do this can leave the board in an inconsistent state.
 * */
void _rr_enemy_reset_pooled(void* item, void* user_data) {
    rrEnemy* enemy = (rrEnemy*)item;
    rrGame* game = (rrGame*)user_data;

    if (!rr_entity_position_is_invalid(&enemy->entity))
        rr_entity_remove_from_grid(&enemy->entity, rr_game_get_grid(game));
}
