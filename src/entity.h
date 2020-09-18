#ifndef RR_ENTITY_H
#define RR_ENTITY_H

#include "point.h"

struct rrGrid;

typedef enum {
    RR_STATUS_INACTIVE,
    RR_STATUS_ACTIVE,
    RR_STATUS_WAITING,
    RR_STATUS_SUSPENDED,
    RR_STATUS_DYING,
    RR_STATUS_KILLED
} rrStatus;

typedef enum {
    RR_ENTITY_PLAYER,
    RR_ENTITY_ENEMY,
    RR_ENTITY_WALL,
    RR_ENTITY_BLOCK,
    RR_ENTITY_CHEESE,
    RR_ENTITY_HOLE,
    RR_ENTITY_UNKNOWN
} rrEntityType;

typedef struct {
    rrPoint position;
    rrStatus status;
    rrEntityType type;
} rrEntity;

void rr_entity_init(rrEntity* entity, rrEntityType type);
void rr_entity_set_invalid_position(rrEntity* entity);
int rr_entity_position_is_invalid(rrEntity* entity);
int rr_entity_is_static(rrEntity* entity);
void rr_entity_deactivate(rrEntity* entity);

/**
 * Moves an entity to a new grid cell which must currently be empty.
 * Convenience method to clear out the old grid location, update entity position, and place it in the target grid location.
 */
void rr_entity_move_to_grid_cell(rrEntity* entity, struct rrGrid* grid, rrPoint* position);

/**
 * Places an entity which is not currently on the grid in the target grid cell which should be empty.
 * Convenience method to update entity position, and place it in the target grid location.
 */
void rr_entity_place_in_grid_cell(rrEntity* entity, struct rrGrid* grid, rrPoint* position);

/**
 * Removes an entity from the grid and sets its position to invalid.
 */
void rr_entity_remove_from_grid(rrEntity* entity, struct rrGrid* grid);

#endif
