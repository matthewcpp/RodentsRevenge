#include "grid.h"

#include <stdio.h>
#include <assert.h>

void rr_grid_init(rrGrid* grid, int width, int height) {
    grid->width = width;
    grid->height = height;
    grid->cells = calloc(width * height, sizeof(rrEntity*));
    grid->_loaded = 0;
}

void rr_grid_uninit(rrGrid* grid) {
    free(grid->cells);
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

/* TODO: pool static entities */
void rr_grid_clear(rrGrid* grid) {
    int i, count = grid->width * grid->height;

    if (!grid->_loaded)
        return;

    for (i = 0; i < count; i++) {
        if (grid->cells[i] == NULL)
            continue;

        if (grid->cells[i]->type != RR_ENTITY_PLAYER && grid->cells[i]->type != RR_ENTITY_ENEMY)
            free(grid->cells[i]);

        grid->cells[i] = NULL;
    }
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
            case 'W':
                entity = malloc(sizeof(rrEntity));
                rr_entity_init(entity, RR_ENTITY_WALL);
                break;

            case 'B':
                entity = malloc(sizeof(rrEntity));
                rr_entity_init(entity, RR_ENTITY_BLOCK);
                break;

            case 'C':
                entity = malloc(sizeof(rrEntity));
                rr_entity_init(entity, RR_ENTITY_CHEESE);
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
    grid->_loaded = 1;
    return 1;
}

void rr_grid_clear_position(rrGrid* grid, rrPoint* position) {
    int index = grid->width * position->y + position->x;
    assert(rr_grid_position_is_valid(grid, position));

    grid->cells[index] = NULL;
}

/* TODO: pool static entities */
rrEntity* rr_grid_create_basic_entity(rrGrid* grid, rrPoint* position, rrEntityType type) {
    int index = grid->width * position->y + position->x;
    rrEntity* entity = NULL;

    assert(type != RR_ENTITY_PLAYER && type != RR_ENTITY_ENEMY);
    assert(rr_grid_position_is_valid(grid, position));

    if (grid->cells[index] == NULL) {
        entity = malloc(sizeof(rrEntity));
        rr_entity_init(entity, type);
        rr_entity_place_in_grid_cell(entity, grid, position);
    }

    return entity;
}

/* TODO: pool static entities */
void rr_grid_destroy_basic_entity(rrGrid* grid, rrEntity* entity) {
    if (rr_grid_position_is_valid(grid, &entity->position))
        rr_grid_clear_position(grid, &entity->position);

    assert(entity->type != RR_ENTITY_PLAYER && entity->type != RR_ENTITY_ENEMY);
    free(entity);
}
