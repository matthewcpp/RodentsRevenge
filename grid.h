#ifndef RR_GRID_H
#define RR_GRID_H

#include <stdlib.h>

#define RR_GRID_WIDTH 23
#define RR_GRID_HEIGHT 23

typedef enum {
    RR_CELL_EMPTY,
    RR_CELL_BLOCK,
    RR_CELL_WALL,
    RR_CELL_MOUSE
} rrCellType;

typedef struct rrGrid {
    unsigned char* cells;
    int width;
    int height;
} rrGrid;

void rr_grid_init(rrGrid* grid, int width, int height);
void rr_grid_uninit(rrGrid* grid);
int rr_grid_position_is_valid(rrGrid* grid, int x, int y);
void rr_grid_set_cell_type(rrGrid* grid, int x, int y, rrCellType type);
rrCellType rr_grid_get_cell_type(rrGrid* grid, int x, int y);

#endif
