#include "entity.h"

void rr_entity_init(rrEntity* entity, rrEntityType type) {
    rr_point_set(&entity->position, 0, 0);
    entity->status = RR_STATUS_INACTIVE;
    entity->type = type;
}
