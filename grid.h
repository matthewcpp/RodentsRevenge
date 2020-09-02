#ifndef RR_GRID_H
#define RR_GRID_H

#include "point.h"

#include <stdlib.h>

#define RR_GRID_WIDTH 23
#define RR_GRID_HEIGHT 23

typedef enum {
    RR_CELL_EMPTY,
    RR_CELL_BLOCK,
    RR_CELL_WALL,
    RR_CELL_COUNT
} rrType;

typedef struct{
    unsigned char type;
    unsigned char flags;
} rrCell;

typedef struct rrGrid {
    rrCell* cells;
    int width;
    int height;
} rrGrid;

void rr_grid_init(rrGrid* grid, int width, int height);
void rr_grid_uninit(rrGrid* grid);
int rr_grid_position_is_valid(rrGrid* grid, rrPoint* position);
rrCell* rr_grid_get_cell(rrGrid* grid, rrPoint* position);
void rr_grid_set_cell_type(rrGrid* grid, rrPoint* position, rrType type);
int rr_grid_load_from_file(rrGrid* grid, const char* path);
int rr_grid_cell_is_blocked(struct rrGrid* grid, rrPoint* position);
void rr_grid_set_cell_blocked(struct rrGrid* grid, rrPoint* position, int blocked);

#endif
