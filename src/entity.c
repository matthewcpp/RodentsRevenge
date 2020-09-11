#include "entity.h"

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
    if (entity->type == RR_ENTITY_PLAYER || entity->type == RR_ENTITY_ENEMY)
        return 0;

    return 1;
}

void rr_entity_deactivate(rrEntity* entity) {
    rr_entity_set_invalid_position(entity);
    entity->status = RR_STATUS_INACTIVE;
}
