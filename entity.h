#ifndef RR_ENTITY_H
#define RR_ENTITY_H

#include "point.h"
#include "grid.h"

typedef enum {
    RR_STATUS_INACTIVE = 0,
    RR_STATUS_ACTIVE = 1,
    RR_STATUS_WAITING = 2,
    RR_STATUS_KILLED = 4
} rrStatus;

typedef enum {
    RR_ENTITY_PLAYER,
    RR_ENTITY_ENEMY
} rrEntityType;

typedef struct {
    rrPoint position;
    rrStatus status;
    rrEntityType type;
    void* ptr;
    rrGrid* _grid;
} rrEntity;

void rr_entity_init(rrEntity* entity, rrEntityType type, void* ptr, rrGrid* grid);
int rr_entity_set_pos(rrEntity* entity, rrPoint* position);

#endif
