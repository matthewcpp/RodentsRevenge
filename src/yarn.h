#ifndef RR_YARN_H
#define RR_YARN_H

#include "draw/renderer.h"
#include "entity.h"
#include "grid.h"

typedef struct {
    rrEntity entity;
    rrGrid* _grid;

    rrPoint direction;
    rrPoint suspended_position;
    int move_count;
    int status_time;
    rrAnimationPlayer explode_animation;
} rrYarn;

void rr_yarn_init(rrYarn* yarn, rrGrid* grid, rrRenderer* renderer);
void rr_yarn_start(rrYarn* yarn, rrPoint* position, rrPoint* direction);
void rr_yarn_update(rrYarn* yarn, int time);

void* _rr_yarn_create_pooled(void* user_data);
void _rr_yarn_reset_pooled(void* item, void* user_data);

#endif
