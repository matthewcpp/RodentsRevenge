#ifndef RR_GRID_H
#define RR_GRID_H

#include "point.h"
#include "entity.h"

#include <stdlib.h>

#define RR_GRID_WIDTH 23
#define RR_GRID_HEIGHT 23

typedef struct rrGrid {
    rrEntity** cells;
    int width;
    int height;
} rrGrid;

void rr_grid_init(rrGrid* grid, int width, int height);
void rr_grid_uninit(rrGrid* grid);
int rr_grid_position_is_valid(rrGrid* grid, rrPoint* position);
rrEntity* rr_grid_get_cell(rrGrid* grid, rrPoint* position);
int rr_grid_load_from_file(rrGrid* grid, const char* path);
int rr_grid_cell_is_blocked(rrGrid* grid, rrPoint* position);
void rr_grid_update_entity_position(rrGrid* grid, rrEntity* entity, rrPoint* position);
void rr_grid_clear_position(rrGrid* grid, rrPoint* position);

#endif
