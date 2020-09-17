#ifndef RR_SPAWNER_H
#define RR_SPAWNER_H

#include "grid.h"
#include "pool.h"

#include "cutil/btree.h"

typedef struct rrSpawner rrSpawner;

rrSpawner* rr_spawner_create(rrGrid* grid, cutil_vector* enemy_list, rrPool* enemy_pool);
void rr_spawner_destroy(rrSpawner* spawner);

int rr_spawner_spawn_enemies(rrSpawner* spawner);
void rr_spawner_get_spawn_pos(rrSpawner* spawner, rrPoint* position);
void rr_spawner_set_properties(rrSpawner* spawner, cutil_btree* properties);

#endif
