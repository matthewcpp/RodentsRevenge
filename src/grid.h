#ifndef RR_GRID_H
#define RR_GRID_H

#include "point.h"
#include "entity.h"

#include <stdlib.h>

#define RR_GRID_WIDTH 23
#define RR_GRID_HEIGHT 23

typedef struct rrGrid rrGrid;

rrGrid* rr_grid_create(int width, int height);
void rr_grid_destroy(rrGrid* grid);
int rr_grid_position_is_valid(rrGrid* grid, rrPoint* position);
rrEntity* rr_grid_get_entity_at_position(rrGrid* grid, rrPoint* position);
void rr_grid_set_entity_at_position(rrGrid* grid, rrEntity* entity, rrPoint* position);
int rr_grid_load_from_file(rrGrid* grid, const char* path);
void rr_grid_clear_position(rrGrid* grid, rrPoint* position);
rrEntity* rr_grid_create_basic_entity(rrGrid* grid, rrPoint* position, rrEntityType type);
void rr_grid_destroy_basic_entity(rrGrid* grid, rrEntity* entity);

#endif
