#ifndef RR_POOL_H
#define RR_POOL_H

#include "cutil/vector.h"

typedef struct rrPool rrPool;

/**
 * Function that creates a new item of type T*
 * This method will be called automatically by the pool when there are no such items in reserve.
 */
typedef void* (*rr_pool_create_func)(void* user_data);

/**
 * Function that resets an item to a default state.
 * This method will be called automatically on all items that are returned to the pool.
 */
typedef void (*rr_pool_reset_func)(void* item, void* user_data);

/**
 * Function that frees all resources for an item in this pool.
 * This method will be called automatically on all items that are in the pool when it is destroyed.
 */
typedef void (*rr_pool_destroy_func)(void* item, void* user_data);

rrPool* rr_pool_create(rr_pool_create_func create_func, rr_pool_reset_func reset_func, rr_pool_destroy_func destroy_func, void* user_data);

/** Destroys the pool and calls the destroy func on every item in reserve. */
void rr_pool_destroy(rrPool* pool);

/** Gets an iem from the pool, or uses the create function to create a new one. */
void* rr_pool_get(rrPool* pool);

/** Returns an item to the pool for reuse later. */
void rr_pool_return(rrPool* pool, void* item);

/** Returns all items in the vector to the pool.
 * It is the callers responsibility to ensure the vector contains items of the correct type.
 * Note that the vector passed to this function is not cleared.
 */
void rr_pool_return_vec(rrPool* pool, cutil_vector* vec);

/** This default destroy function simply calls free on the item passed to it. */
void rr_pool_default_delete_func(void* item, void* user_data);

/** The default reset function is a no-op. */
void rr_pool_default_reset_func(void* item, void* user_data);

#endif
