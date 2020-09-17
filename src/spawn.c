#include "spawn.h"

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

typedef enum {
    RR_SPAWNER_MODE_CYCLE,
    RR_SPAWNER_MODE_CONSTANT
} rrSpawnerMode;

struct rrSpawner {
    rrGrid* grid;
    cutil_vector* enemy_list;
    rrEnemyPool* enemy_pool;

    int spawn_count;
    cutil_vector* spawn_cycle;
    rrSpawnerMode mode;
};

rrSpawner* rr_spawner_create(rrGrid* grid, cutil_vector* enemy_list, rrEnemyPool* enemy_pool) {
    rrSpawner* spawner = malloc(sizeof(rrSpawner));

    spawner->grid = grid;
    spawner->enemy_list = enemy_list;
    spawner->enemy_pool = enemy_pool;
    spawner->spawn_count = 1;
    spawner->spawn_cycle = cutil_vector_create(cutil_trait_int());
    spawner->mode = RR_SPAWNER_MODE_CONSTANT;

    return spawner;
}

void rr_spawner_destroy(rrSpawner* spawner) {
    cutil_vector_destroy(spawner->spawn_cycle);
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

void rr_spawner_post_spawn(rrSpawner* spawner) {
    if (spawner->mode == RR_SPAWNER_MODE_CYCLE){
        spawner->spawn_count += 1;

        if (spawner->spawn_count >= cutil_vector_size(spawner->spawn_cycle))
            spawner->spawn_count = 0;
    }
}

int rr_spawner_get_spawn_count(rrSpawner* spawner) {
    switch(spawner->mode) {
        case RR_SPAWNER_MODE_CONSTANT:
            return spawner->spawn_count;

        case RR_SPAWNER_MODE_CYCLE: {
            int count;
            cutil_vector_get(spawner->spawn_cycle, spawner->spawn_count, &count);
            return count;
        }

        default:
            return 0;
    }
}

int rr_spawner_spawn_enemies(rrSpawner* spawner) {
    int spawn_count = rr_spawner_get_spawn_count(spawner);
    int i;

    for (i = 0; i < spawn_count; i++) {
        rrEnemy* enemy = rr_enemy_pool_get(spawner->enemy_pool);
        rrPoint spawn_pos;

        rr_spawner_get_spawn_pos(spawner, &spawn_pos);
        rr_entity_place_in_grid_cell(&enemy->entity, spawner->grid, &spawn_pos);
        enemy->entity.status = RR_STATUS_ACTIVE;

        cutil_vector_push_back(spawner->enemy_list, &enemy);
    }

    rr_spawner_post_spawn(spawner);

    return spawn_count;
}

void rr_spawner_parse_cycle_props(rrSpawner* spawner, const char* props) {
    char* end_ptr;
    int val = (int)strtol(props, &end_ptr, 10);

    while (val != 0) {
        cutil_vector_push_back(spawner->spawn_cycle, &val);
        props = end_ptr;
        val = (int)strtol(props, &end_ptr, 10);
    }

    assert(cutil_vector_size(spawner->spawn_cycle) > 0);
    spawner->spawn_count = 0;
    spawner->mode = RR_SPAWNER_MODE_CYCLE;
}

void rr_spawner_parse_constant_props(rrSpawner* spawner, const char* props){
    spawner->mode = RR_SPAWNER_MODE_CONSTANT;
    spawner->spawn_count = strtol(props, NULL, 10);
}

void rr_spawner_set_properties(rrSpawner* spawner, cutil_btree* properties) {
    char* key = "spawn";
    char* spawner_props = NULL;

    cutil_btree_get(properties, &key, &spawner_props);

    spawner->mode = RR_SPAWNER_MODE_CONSTANT;
    spawner->spawn_count = 1;
    cutil_vector_clear(spawner->spawn_cycle);

    if (spawner_props == NULL)
        return;

    if (strstr(spawner_props, "constant") == spawner_props) {
        rr_spawner_parse_constant_props(spawner, spawner_props + 8);
    }
    else if (strstr(spawner_props, "cycle") == spawner_props) {
        rr_spawner_parse_cycle_props(spawner, spawner_props + 6);
    }
}
