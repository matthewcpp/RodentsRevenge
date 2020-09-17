#ifndef RR_POOL_H
#define RR_POOL_H

#include "enemy.h"

#include "cutil/vector.h"

typedef struct rrEnemyPool rrEnemyPool;

rrEnemyPool* rr_enemy_pool_create(rrEntity* player, struct rrGrid* grid);

/** Destroys the pool and deletes every enemy in reserve. */
void rr_enemy_pool_destroy(rrEnemyPool* pool);

/** Gets an enemy from the pool.  If one is not available in reserve a new one will be created. */
rrEnemy* rr_enemy_pool_get(rrEnemyPool* pool);

/** Returns an enemy to the pool so it can be re-used later. */
void rr_enemy_pool_return(rrEnemyPool* pool, rrEnemy* enemy);

/**
 * Returns all enemies in the vector to the pull.
 * It is the callers responsibility to ensure the vector contains only enemies.
 * Note: This method does not clear the supplied vector.
 */
void rr_enemy_pool_return_vec(rrEnemyPool* pool, cutil_vector* enemy_vector);

#endif
