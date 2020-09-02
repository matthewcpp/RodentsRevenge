#include "entity.h"

void rr_entity_init(rrEntity* entity, rrEntityType type, void* ptr, rrGrid* grid) {
    rr_point_set(&entity->position, 0, 0);
    entity->status = RR_STATUS_INACTIVE;
    entity->type = type;
    entity->ptr = ptr;
    entity->_grid = grid;
}

int rr_entity_set_pos(rrEntity* entity, rrPoint* position) {
    if (!rr_grid_position_is_valid(entity->_grid, position))
        return 0;

    rr_point_copy(&entity->position, position);

    return 1;
}