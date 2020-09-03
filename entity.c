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

    if (entity->status != RR_STATUS_INACTIVE)
        rr_grid_set_cell_blocked(entity->_grid, &entity->position, 0);

    rr_point_copy(&entity->position, position);
    rr_grid_set_cell_blocked(entity->_grid, &entity->position, 1);

    return 1;
}
