#include "spawner.h"

#include "enemy.h"

#include "cutil/strbuf.h"
#include "cutil/vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define RR_SPAWN_POS_COUNT 8

rrPoint spawn_positions[RR_SPAWN_POS_COUNT] = {
        {1, 1}, {21, 1}, {21, 1}, {21, 21},
        {11, 1}, {21, 11}, {11, 21}, {1, 11}
};

struct rrSpawner {
    rrGrid* grid;
    rrPool* enemy_pool;
    rrPool* yarn_pool;

    int enemy_spawn_index;
    cutil_vector* enemy_spawn_cycle;
};

rrSpawner* rr_spawner_create(rrGrid* grid, rrPool* enemy_pool, rrPool* yarn_pool) {
    rrSpawner* spawner = malloc(sizeof(rrSpawner));

    spawner->grid = grid;
    spawner->enemy_pool = enemy_pool;
    spawner->yarn_pool = yarn_pool;
    spawner->enemy_spawn_index = 0;
    spawner->enemy_spawn_cycle = cutil_vector_create(cutil_trait_int());

    return spawner;
}

void rr_spawner_destroy(rrSpawner* spawner) {
    cutil_vector_destroy(spawner->enemy_spawn_cycle);
    free(spawner);
}

void randomize_index_array(int* spawn_points, int size) {
    int i;

    for (i = 0; i < size; i++)
        spawn_points[i] = i;

    for (i = 0; i < size; i++) {
        int swap_index = rand() % size;
        int temp = spawn_points[i];
        spawn_points[i] = spawn_points[swap_index];
        spawn_points[swap_index] = temp;
    }
}

void rr_spawner_get_spawn_pos(rrSpawner* spawner, rrPoint* position) {
    int i;
    int spawn_points[RR_SPAWN_POS_COUNT];
    randomize_index_array(spawn_points, RR_SPAWN_POS_COUNT);

    for (i = 0; i < RR_SPAWN_POS_COUNT; i++) {
        rrPoint* pos = spawn_positions + spawn_points[i];
        if (rr_grid_get_entity_at_position(spawner->grid, pos) == NULL){
            rr_point_copy(position, pos);
            break;
        }
    }
}

int rr_spawner_spawn_enemies(rrSpawner* spawner, cutil_vector* enemy_list) {
    int spawn_count;
    int i;

    cutil_vector_get(spawner->enemy_spawn_cycle, spawner->enemy_spawn_index, &spawn_count);

    for (i = 0; i < spawn_count; i++) {
        rrEnemy* enemy = rr_pool_get(spawner->enemy_pool);
        rrPoint spawn_pos;

        rr_spawner_get_spawn_pos(spawner, &spawn_pos);
        rr_entity_place_in_grid_cell(&enemy->entity, spawner->grid, &spawn_pos);
        enemy->entity.status = RR_STATUS_ACTIVE;

        cutil_vector_push_back(enemy_list, &enemy);
    }

    spawner->enemy_spawn_index += 1;
    if (spawner->enemy_spawn_index >= cutil_vector_size(spawner->enemy_spawn_cycle))
        spawner->enemy_spawn_index = 0;

    return spawn_count;
}

void rr_spawner_parse_spawn_values(rrSpawner* spawner, const char* props) {
    char* end_ptr;
    int val = (int)strtol(props, &end_ptr, 10);

    while (val != 0) {
        cutil_vector_push_back(spawner->enemy_spawn_cycle, &val);
        props = end_ptr;
        val = (int)strtol(props, &end_ptr, 10);
    }

    assert(cutil_vector_size(spawner->enemy_spawn_cycle) > 0);
    spawner->enemy_spawn_index = 0;
}


void rr_spawner_set_properties(rrSpawner* spawner, cutil_btree* properties) {
    char* key = "spawn";
    char* spawner_props = NULL;

    if (!cutil_btree_get(properties, &key, &spawner_props))
        return;

    spawner->enemy_spawn_index = 0;
    cutil_vector_clear(spawner->enemy_spawn_cycle);

    /* no properties set use default. */
    if (spawner_props == NULL){
        int val = 1;
        cutil_vector_push_back(spawner->enemy_spawn_cycle, &val);
        return;
    }

    rr_spawner_parse_spawn_values(spawner, spawner_props);
}
