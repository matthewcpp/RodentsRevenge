#include "yarn.h"

#include "game.h"
#include "util.h"

#include <assert.h>

#define RR_YARN_WAIT_TIME_MIN 1000
#define RR_YARN_WAIT_TIME_MAX 10000
#define RR_YARN_MOVE_TIME 100

void _rr_yarn_reset(rrYarn* yarn);

void rr_yarn_init(rrYarn* yarn, rrGrid* grid) {
    rr_entity_init(&yarn->entity, RR_ENTITY_YARN);
    yarn->_grid = grid;
    _rr_yarn_reset(yarn);
}

void rr_yarn_start(rrYarn* yarn, rrPoint* position, rrPoint* direction) {
    rrEntity* target = rr_grid_get_entity_at_position(yarn->_grid, position);

    /* yarn must start on a wall tile */
    assert(target != NULL && target->type == RR_ENTITY_WALL);

    rr_grid_destroy_basic_entity(yarn->_grid, target);

    rr_entity_place_in_grid_cell(&yarn->entity, yarn->_grid, position);
    rr_point_copy(&yarn->direction, direction);
    yarn->entity.status = RR_STATUS_WAITING;
    yarn->status_time = rand_between(RR_YARN_WAIT_TIME_MIN, RR_YARN_WAIT_TIME_MAX);
}

void rr_yarn_update_waiting(rrYarn* yarn, int time) {
    yarn->status_time -= time;

    if (yarn->status_time <= 0) {
        yarn->entity.status = RR_STATUS_ACTIVE;
        yarn->status_time = 0;
    }
}

void rr_yarn_update_active(rrYarn* yarn, int time) {
    rrPoint target_pos, current_pos;
    rrEntity* target_entity;
    yarn->status_time += time;

    if (yarn->status_time < RR_YARN_MOVE_TIME)
        return;

    rr_point_add(&target_pos, &yarn->entity.position, &yarn->direction);
    target_entity = rr_grid_get_entity_at_position(yarn->_grid, & target_pos);
    if (target_entity == NULL) {
        rr_point_copy(&current_pos, &yarn->entity.position);
        rr_entity_move_to_grid_cell(&yarn->entity, yarn->_grid, &target_pos);

        if (yarn->move_count == 0)
            rr_grid_create_basic_entity(yarn->_grid, &current_pos, RR_ENTITY_WALL);

        yarn->move_count += 1;
    }
    else {
        yarn->entity.status = RR_STATUS_STUCK;
    }

    yarn->status_time = 0;
}

void rr_yarn_update(rrYarn* yarn, int time) {
    switch (yarn->entity.status) {
        case RR_STATUS_WAITING:
            rr_yarn_update_waiting(yarn, time);
            break;

        case RR_STATUS_ACTIVE:
            rr_yarn_update_active(yarn, time);
            break;

        default:
            break;
    }
}

void _rr_yarn_reset(rrYarn* yarn) {
    yarn->_collision = NULL;
    yarn->move_count = 0;
    yarn->status_time = 0;
    rr_point_set(&yarn->direction, 0, 0);
}

void* _rr_yarn_create_pooled(void* user_data){
    rrGame* game = (rrGame*)user_data;
    rrYarn* yarn = malloc(sizeof(rrYarn));
    rr_yarn_init(yarn, rr_game_get_grid(game));

    return yarn;
}

void _rr_yarn_reset_pooled(void* item, void* user_data) {
    rrYarn* yarn = (rrYarn*)item;
    rrGame* game = (rrGame*)user_data;

    _rr_yarn_reset(yarn);
    if (!rr_entity_position_is_invalid(&yarn->entity))
        rr_entity_remove_from_grid(&yarn->entity, rr_game_get_grid(game));
}
