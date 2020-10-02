#include "entity.h"
#include "grid.h"

#include <assert.h>

void rr_entity_init(rrEntity* entity, rrEntityType type) {
    rr_entity_deactivate(entity);
    entity->type = type;
}

void rr_entity_set_invalid_position(rrEntity* entity) {
    entity->position.x = -1;
    entity->position.y = -1;
}

int rr_entity_position_is_invalid(rrEntity* entity) {
    return entity->position.x == -1 && entity->position.y == -1;
}

int rr_entity_is_static(rrEntity* entity) {
    if (entity->type == RR_ENTITY_PLAYER || entity->type == RR_ENTITY_ENEMY || entity->type == RR_ENTITY_YARN)
        return 0;

    return 1;
}

void rr_entity_deactivate(rrEntity* entity) {
    rr_entity_set_invalid_position(entity);
    entity->status = RR_STATUS_INACTIVE;
}

void rr_entity_move_to_grid_cell(rrEntity* entity, rrGrid* grid, rrPoint* position) {
    rr_grid_clear_position(grid, &entity->position);
    rr_point_copy(&entity->position, position);
    rr_grid_set_entity_at_position(grid, entity, position);
}

void rr_entity_place_in_grid_cell(rrEntity* entity, rrGrid* grid, rrPoint* position) {
    assert(rr_entity_position_is_invalid(entity));
    rr_point_copy(&entity->position, position);
    rr_grid_set_entity_at_position(grid, entity, position);
}

void rr_entity_remove_from_grid(rrEntity* entity, rrGrid* grid) {
    assert(!rr_entity_position_is_invalid(entity));

    rr_grid_clear_position(grid, &entity->position);
    rr_entity_deactivate(entity);
}
