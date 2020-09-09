#ifndef RR_ENTITY_H
#define RR_ENTITY_H

#include "point.h"

typedef enum {
    RR_STATUS_INACTIVE = 0,
    RR_STATUS_ACTIVE = 1,
    RR_STATUS_WAITING = 2,
    RR_STATUS_KILLED = 4
} rrStatus;

typedef enum {
    RR_ENTITY_PLAYER,
    RR_ENTITY_ENEMY,
    RR_ENTITY_WALL,
    RR_ENTITY_BLOCK,
    RR_ENTITY_CHEESE
} rrEntityType;

typedef struct {
    rrPoint position;
    rrStatus status;
    rrEntityType type;
} rrEntity;

void rr_entity_init(rrEntity* entity, rrEntityType type);
void rr_entity_set_invalid_position(rrEntity* entity);
int rr_entity_position_is_invalid(rrEntity* entity);

#endif
