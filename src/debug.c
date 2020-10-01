#include "debug.h"

#include "game_private.h"

#include <stdio.h>
#include <string.h>

void rr_debug_spawn_yarn(rrGame* game, const char* str) {
    rrPoint position, direction;
    int status_time;
    rrYarn* yarn =  rr_pool_get(game->_yarn_pool);

    sscanf(str, "%d %d %d %d %d", &position.x, & position.y, &direction.x, &direction.y, &status_time);
    rr_yarn_start(yarn, &position, &direction);
    yarn->status_time = status_time;
    cutil_vector_push_back(game->_yarns, &yarn);
}

void rr_debug_spawn_enemy(rrGame* game, const char* str) {
    rrEnemy* enemy = rr_pool_get(game->_enemy_pool);
    rrPoint position;
    sscanf(str, "%d %d", &position.x, & position.y);

    rr_entity_place_in_grid_cell(&enemy->entity, enemy->_grid, &position);
    enemy->entity.status = RR_STATUS_ACTIVE;
}

void rr_debug_set_enemy_freq(rrGame* game, const char* str) {
    int enemy_freq;
    sscanf(str, "%d", &enemy_freq);
    game->enemy_update_frequency = enemy_freq;
}

void rr_debug_load_properties(rrGame* game) {
    cutil_btree* properties = rr_grid_get_properties(game->grid);
    cutil_btree_itr* itr = cutil_btree_itr_create(properties);

    while (cutil_btree_itr_next(itr)) {
        char* key;
        char* value;

        cutil_btree_itr_get_key(itr, &key);
        cutil_btree_itr_get_value(itr, &value);

        if (strstr(key, "yarn") == key)
            rr_debug_spawn_yarn(game, value);
        if (strstr(key, "enemyfreq") == key)
            rr_debug_set_enemy_freq(game, value);
        else if (strstr(key, "enemy") == key)
            rr_debug_spawn_enemy(game, value);
    }
}
