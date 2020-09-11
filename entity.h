#ifndef RR_ENTITY_H
#define RR_ENTITY_H

#include "point.h"

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
int rr_entity_is_static(rrEntity* entity);
void rr_entity_deactivate(rrEntity* entity);

#endif
