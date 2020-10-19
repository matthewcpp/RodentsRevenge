#include "grid.h"

#include "pool.h"

#include "cutil/strbuf.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

struct rrGrid{
    rrEntity** cells;
    int width;
    int height;
    int loaded;
    cutil_btree* properties;
    rrPool* static_pool;
};

void* rr_grid_create_pooled_entity(void* user_data) {
    rrEntity * entity = malloc(sizeof(rrEntity));
    rr_entity_init(entity, RR_ENTITY_UNKNOWN);
    (void)user_data;
    return entity;
}

void rr_grid_reset_pool_entity(void* item, void* user_data) {
    (void)user_data;
    assert(rr_entity_is_static(item));
    rr_entity_init(item, RR_ENTITY_UNKNOWN);
}

rrGrid* rr_grid_create(int width, int height) {
    rrGrid* grid = malloc(sizeof(struct rrGrid));

    grid->width = width;
    grid->height = height;
    grid->cells = calloc(width * height, sizeof(rrEntity*));
    grid->loaded = 0;
    grid->properties = cutil_btree_create(4, cutil_trait_cstring(), cutil_trait_cstring());
    grid->static_pool = rr_pool_create(rr_grid_create_pooled_entity, rr_grid_reset_pool_entity, rr_pool_default_delete_func, NULL);

    return grid;
}

void rr_grid_clear(rrGrid* grid) {
    int i, count = grid->width * grid->height;

    if (!grid->loaded)
        return;

    for (i = 0; i < count; i++) {
        rrEntity* entity = grid->cells[i];
        if (entity == NULL || !rr_entity_is_static(entity))
            continue;

        rr_pool_return(grid->static_pool, grid->cells[i]);
    }

    memset(grid->cells, 0, count * sizeof(rrEntity*));
    grid->loaded = 0;
}

void rr_grid_destroy(rrGrid* grid) {
    rr_grid_clear(grid);
    rr_pool_destroy(grid->static_pool);
    cutil_btree_destroy(grid->properties);
    free(grid->cells);
    free(grid);
}

int rr_grid_position_is_valid(rrGrid* grid, rrPoint* position) {
    return position->x >= 0 && position->x < grid->width && position->y >= 0 && position->y < grid->height;
}

rrEntity* rr_grid_get_entity_at_position(rrGrid* grid, rrPoint* position) {
    assert(rr_grid_position_is_valid(grid, position));

    return grid->cells[grid->width * position->y + position->x];
}

void rr_grid_set_entity_at_position(rrGrid* grid, rrEntity* entity, rrPoint* position) {
    assert(rr_grid_position_is_valid(grid, position));

    grid->cells[grid->width * position->y + position->x] = entity;
}

void rr_grid_parse_property(rrGrid* grid, FILE* file) {
    cutil_strbuf* property_name = cutil_strbuf_create();
    cutil_strbuf* property_value = cutil_strbuf_create();
    char* name_ptr;
    char* value_ptr;

    int ch = fgetc(file);

    /* read prop name */
    while (ch != ' ') {
        cutil_strbuf_append_char(property_name, (char)ch);
        ch = fgetc(file);
    }

    /* read prop value */
    ch = fgetc(file);
    while (ch != '\n') {
        if (ch != '\r')
            cutil_strbuf_append_char(property_value, (char)ch);

        ch = fgetc(file);
    }

    name_ptr = (char*)cutil_strbuf_cstring(property_name);
    value_ptr = (char*)cutil_strbuf_cstring(property_value);

    cutil_btree_insert(grid->properties, &name_ptr, &value_ptr);

    cutil_strbuf_destroy(property_name);
    cutil_strbuf_destroy(property_value);
}

int rr_grid_load_from_file(rrGrid* grid, const char* path) {
    rrPoint cell;
    FILE* file = fopen(path, "r");

    if (!file)
        return 0;

    rr_grid_clear(grid);

    rr_point_set(&cell, 0, 0);

    for (;;) {
        int c = fgetc(file);
        rrEntity* entity = NULL;

        if (c == EOF)
            break;

        switch (c) {
            case '#':
                rr_grid_parse_property(grid, file);
                continue;

            case 'W':
                entity = rr_pool_get(grid->static_pool);
                entity->type = RR_ENTITY_WALL;
                break;

            case 'B':
                entity = rr_pool_get(grid->static_pool);
                entity->type = RR_ENTITY_BLOCK;
                break;

            case 'C':
                entity = rr_pool_get(grid->static_pool);
                entity->type = RR_ENTITY_CHEESE;
                break;

            case 'H':
                entity = rr_pool_get(grid->static_pool);
                entity->type = RR_ENTITY_HOLE;
                break;

            case 'T':
                entity = rr_pool_get(grid->static_pool);
                entity->type = RR_ENTITY_TRAP;
                break;

            case ' ':
                break;

            default:
                continue;
        }

        if (entity){
            entity->status = RR_STATUS_ACTIVE;
            rr_entity_place_in_grid_cell(entity, grid, &cell);
        }

        cell.x += 1;
        if (cell.x == RR_GRID_WIDTH) {
            cell.y += 1;
            cell.x = 0;
        }
    }

    fclose(file);
    grid->loaded = 1;
    return 1;
}

void rr_grid_clear_position(rrGrid* grid, rrPoint* position) {
    int index = grid->width * position->y + position->x;
    assert(rr_grid_position_is_valid(grid, position));

    grid->cells[index] = NULL;
}

rrEntity* rr_grid_create_basic_entity(rrGrid* grid, rrPoint* position, rrEntityType type) {
    int index = grid->width * position->y + position->x;
    rrEntity* entity = NULL;

    assert(type != RR_ENTITY_PLAYER && type != RR_ENTITY_ENEMY);
    assert(rr_grid_position_is_valid(grid, position));

    if (grid->cells[index] == NULL) {
        entity = rr_pool_get(grid->static_pool);
        entity->type = type;
        rr_entity_place_in_grid_cell(entity, grid, position);
    }

    return entity;
}

void rr_grid_destroy_basic_entity(rrGrid* grid, rrEntity* entity) {
    assert(rr_entity_is_static(entity));
    if (rr_grid_position_is_valid(grid, &entity->position))
        rr_grid_clear_position(grid, &entity->position);

    rr_pool_return(grid->static_pool, entity);
}

cutil_btree* rr_grid_get_properties(rrGrid* grid) {
    return grid->properties;
}

void rr_grid_get_size(rrGrid* grid, rrPoint* size) {
    size->x = grid->width;
    size->y = grid->height;
}
