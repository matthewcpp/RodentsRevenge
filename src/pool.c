#include "pool.h"

#include <stdlib.h>

struct rrPool {
    rr_pool_create_func create_func;
    rr_pool_reset_func reset_func;
    rr_pool_destroy_func destroy_func;
    void* user_data;
    cutil_vector* reserve;
};

rrPool* rr_pool_create(rr_pool_create_func create_func, rr_pool_reset_func reset_func, rr_pool_destroy_func destroy_func, void* user_data) {
    rrPool* pool = malloc(sizeof(rrPool));

    pool->create_func = create_func;
    pool->reset_func = reset_func;
    pool->destroy_func = destroy_func;
    pool->user_data = user_data;
    pool->reserve = cutil_vector_create(cutil_trait_ptr());

    return pool;
}

void rr_pool_destroy(rrPool* pool) {
    size_t i;
    size_t size = cutil_vector_size(pool->reserve);
    void* item;

    for (i = 0; i < size; i++) {
        cutil_vector_get(pool->reserve, i , &item);
        pool->destroy_func(item, pool->user_data);
    }

    cutil_vector_destroy(pool->reserve);
    free(pool);
}

void* rr_pool_get(rrPool* pool) {
    void* item;

    if (!cutil_vector_empty(pool->reserve)) {
        cutil_vector_get(pool->reserve, cutil_vector_size(pool->reserve) - 1, &item);
        cutil_vector_pop_back(pool->reserve);
    }
    else {
        item = pool->create_func(pool->user_data);
    }

    return item;
}

void rr_pool_return(rrPool* pool, void* item) {
    pool->reset_func(item, pool->user_data);
    cutil_vector_push_back(pool->reserve, &item);
}

void rr_pool_return_vec(rrPool* pool, cutil_vector* vec) {
    size_t i;
    size_t count = cutil_vector_size(vec);

    for (i = 0; i < count; i++) {
        void* item;
        cutil_vector_get(vec, i, &item);
        rr_pool_return(pool, item);
    }
}

void rr_pool_default_delete_func(void* item, void* user_data) {
    (void)user_data;
    free(item);
}

void rr_pool_default_reset_func(void* item, void* user_data) {
    (void)item;
    (void)user_data;
}
