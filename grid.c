#include "grid.h"

void rr_grid_init(rrGrid* grid, int width, int height) {
    grid->width = width;
    grid->height = height;
    grid->cells = calloc(width * height, sizeof(unsigned char));
}

void rr_grid_uninit(rrGrid* grid) {
    free(grid->cells);
}

int rr_grid_position_is_valid(rrGrid* grid, int x, int y) {
    return x >= 0 && x < grid->width && y >= 0 && y < grid->height;
}

void rr_grid_set_cell_type(rrGrid* grid, int x, int y, rrCellType type) {
    grid->cells[grid->width * y + x] = type;
}

rrCellType rr_grid_get_cell_type(rrGrid* grid, int x, int y) {
    return grid->cells[grid->width * y + x];
}