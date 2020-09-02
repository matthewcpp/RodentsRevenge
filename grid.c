#include "grid.h"

#include <stdio.h>
#include <assert.h>

#define RR_CELL_BLOCK_BIT 1U

void rr_grid_init(rrGrid* grid, int width, int height) {
    grid->width = width;
    grid->height = height;
    grid->cells = calloc(width * height, sizeof(rrCell));
}

void rr_grid_uninit(rrGrid* grid) {
    free(grid->cells);
}

int rr_grid_position_is_valid(rrGrid* grid, rrPoint* position) {
    return position->x >= 0 && position->x < grid->width && position->y >= 0 && position->y < grid->height;
}

rrCell* rr_grid_get_cell(rrGrid* grid, rrPoint* position) {
    assert(rr_grid_position_is_valid(grid, position));

    return &grid->cells[grid->width * position->y + position->x];
}

void rr_grid_set_cell_type(rrGrid* grid, rrPoint* position, rrType type) {
    int blocked = (type == RR_CELL_WALL || type == RR_CELL_BLOCK);

    grid->cells[grid->width * position->y + position->x].type = type;
    rr_grid_set_cell_blocked(grid, position, blocked);
}

int rr_grid_load_from_file(rrGrid* grid, const char* path) {
    int i = 0;
    FILE* file = fopen(path, "r");

    if (!file)
        return 0;

    while (1) {
        int c = fgetc(file);

        if (c == EOF)
            break;

        switch (c) {
            case 'W':
                grid->cells[i].type = RR_CELL_WALL;
                grid->cells[i++].flags = RR_CELL_BLOCK_BIT;
                break;
            case 'B':
                grid->cells[i].type = RR_CELL_BLOCK;
                grid->cells[i++].flags = RR_CELL_BLOCK_BIT;
                break;
            case ' ':
                grid->cells[i++].type = RR_CELL_EMPTY;
                break;
            default:
                continue;
        }
    }

    fclose(file);
    return 1;
}

int rr_grid_cell_is_blocked(struct rrGrid* grid, rrPoint* position) {
    assert(rr_grid_position_is_valid(grid, position));

    return (grid->cells[grid->width * position->y + position->x].flags & (RR_CELL_BLOCK_BIT)) == RR_CELL_BLOCK_BIT;
}

void rr_grid_set_cell_blocked(struct rrGrid* grid, rrPoint* position, int blocked) {
    int index = grid->width * position->y + position->x;

    assert(rr_grid_position_is_valid(grid, position));

    if (blocked)
        grid->cells[index].flags = grid->cells[index].flags | RR_CELL_BLOCK_BIT;

    else
        grid->cells[index].flags = grid->cells[index].flags & ~RR_CELL_BLOCK_BIT;
}
