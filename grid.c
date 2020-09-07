#include "grid.h"

#include <stdio.h>
#include <assert.h>

void rr_grid_init(rrGrid* grid, int width, int height) {
    grid->width = width;
    grid->height = height;
    grid->cells = calloc(width * height, sizeof(rrEntity*));
}

void rr_grid_uninit(rrGrid* grid) {
    free(grid->cells);
}

int rr_grid_position_is_valid(rrGrid* grid, rrPoint* position) {
    return position->x >= 0 && position->x < grid->width && position->y >= 0 && position->y < grid->height;
}

rrEntity* rr_grid_get_cell(rrGrid* grid, rrPoint* position) {
    assert(rr_grid_position_is_valid(grid, position));

    return grid->cells[grid->width * position->y + position->x];
}

int rr_grid_load_from_file(rrGrid* grid, const char* path) {
    rrPoint cell;
    rr_point_set(&cell, 0, 0);
    FILE* file = fopen(path, "r");

    if (!file)
        return 0;

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

            case ' ':
                break;

            default:
                continue;
        }

        if (entity)
            rr_grid_update_entity_position(grid, entity, &cell);

        cell.x += 1;
        if (cell.x == RR_GRID_WIDTH) {
            cell.y += 1;
            cell.x = 0;
        }
    }

    fclose(file);
    return 1;
}

int rr_grid_cell_is_blocked(struct rrGrid* grid, rrPoint* position) {
    int index = grid->width * position->y + position->x;
    assert(rr_grid_position_is_valid(grid, position));

    if (grid->cells[index] == NULL)
        return 0;
    else
        return 1;
}

void rr_grid_clear_position(rrGrid* grid, rrPoint* position) {
    int index = grid->width * position->y + position->x;
    assert(rr_grid_position_is_valid(grid, position));

    grid->cells[index] = NULL;
}

void rr_grid_update_entity_position(rrGrid* grid, rrEntity* entity, rrPoint* position) {
    int src_index = grid->width * entity->position.y + entity->position.x;
    int dest_index = grid->width * position->y + position->x;

    assert(rr_grid_position_is_valid(grid, position));
    //assert(grid->cells[dest_index] == NULL);

    /* entities are initially created with an invalid position off the board and must be explicitly placed */
    if (!rr_entity_position_is_invalid(entity))
        grid->cells[src_index] = NULL;

    rr_point_copy(&entity->position, position);
    grid->cells[dest_index] = entity;
}