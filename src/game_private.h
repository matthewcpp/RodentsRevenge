#ifndef RR_GAME_PRIVATE_H
#define RR_GAME_PRIVATE_H

#include "enemy.h"
#include "pool.h"

#include "spawner.h"
#include "util.h"
#include "yarn.h"

struct rrGame {
    rrGrid* grid;
    rrPlayer player;
    rrClock clock;
    rrGameState state;

    cutil_vector* _enemies;
    cutil_vector* _yarns;
    rrPool* _enemy_pool;
    rrPool* _yarn_pool;
    rrInput* _input;
    rrRenderer* _renderer;
    rrSpawner* _spawner;
    int current_level;

    int enemy_update_frequency;
    int enemy_update_time;

    int yarn_spawn_time_min;
    int yarn_spawn_time_max;
    int yarn_spawn_time;

    char* _asset_path;
    size_t _asset_path_len;
};

#endif
