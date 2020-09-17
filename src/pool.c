#include "pool.h"

#include "cutil/vector.h"

struct rrEnemyPool {
    cutil_vector* reserve;
    rrEntity* player;
    rrGrid* grid;
};

rrEnemyPool* rr_enemy_pool_create(rrEntity* player, rrGrid* grid) {
    rrEnemyPool* pool = malloc(sizeof(rrEnemyPool));

    pool->reserve = cutil_vector_create(cutil_trait_ptr());
    pool->grid = grid;
    pool->player = player;

    return pool;
}

void rr_enemy_pool_destroy(rrEnemyPool* pool) {
    size_t i;
    rrEnemy* enemy;

    for (i = 0; i < cutil_vector_size(pool->reserve); i++) {
        cutil_vector_get(pool->reserve, i, &enemy);
        free(enemy);
    }

    cutil_vector_destroy(pool->reserve);
    free(pool);
}

rrEnemy* rr_enemy_pool_get(rrEnemyPool* pool) {
    rrEnemy* enemy;
    size_t size = cutil_vector_size(pool->reserve);
    if (size > 0) {
        cutil_vector_get(pool->reserve, size - 1, &enemy);
        cutil_vector_pop_back(pool->reserve);
    }
    else {
        enemy = malloc(sizeof(rrEnemy));
        rr_enemy_init(enemy, pool->player, pool->grid);
    }

    return enemy;
}

void rr_enemy_pool_return(rrEnemyPool* pool, rrEnemy* enemy) {
    cutil_vector_push_back(pool->reserve, &enemy);
}

void rr_enemy_pool_return_vec(rrEnemyPool* pool, cutil_vector* enemy_vector) {
    size_t i;

    for (i = 0; i < cutil_vector_size(enemy_vector); i++) {
        rrEnemy* enemy;
        cutil_vector_get(enemy_vector, i, &enemy);
        rr_enemy_pool_return(pool, enemy);
    }
}
