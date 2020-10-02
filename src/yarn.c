#include "yarn.h"

#include "assets.h"
#include "game.h"
#include "util.h"

#include <assert.h>

#define RR_YARN_WAIT_TIME_MIN 1000
#define RR_YARN_WAIT_TIME_MAX 5000
#define RR_YARN_MOVE_TIME 100

void _rr_yarn_reset(rrYarn* yarn);

void rr_yarn_init(rrYarn* yarn, rrGrid* grid, rrRenderer* renderer) {
    rr_entity_init(&yarn->entity, RR_ENTITY_YARN);
    yarn->_grid = grid;
    rr_animation_player_init(&yarn->explode_animation, rr_renderer_get_animation(renderer, RR_ANIMATION_YARN_EXPLODE));
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
        yarn->status_time = RR_YARN_MOVE_TIME;
    }
}

void rr_yarn_explode(rrYarn* yarn) {
    yarn->entity.status = RR_STATUS_DYING;
    rr_animation_player_reset(&yarn->explode_animation);
}

void rr_yarn_update_active(rrYarn* yarn, int time) {
    rrPoint target_pos, current_pos;
    rrEntity* target_entity;
    yarn->status_time -= time;

    if (yarn->status_time > 0)
        return;

    rr_point_add(&target_pos, &yarn->entity.position, &yarn->direction);
    target_entity = rr_grid_get_entity_at_position(yarn->_grid, &target_pos);
    if (target_entity == NULL) {
        rr_point_copy(&current_pos, &yarn->entity.position);
        rr_entity_move_to_grid_cell(&yarn->entity, yarn->_grid, &target_pos);

        if (yarn->move_count == 0)
            rr_grid_create_basic_entity(yarn->_grid, &current_pos, RR_ENTITY_WALL);

        yarn->move_count += 1;
    }
    else if (target_entity->type == RR_ENTITY_PLAYER) {
        rr_player_kill((rrPlayer*)target_entity, NULL);
        yarn->entity.status = RR_STATUS_KILLED;
    }
    else if (target_entity->type == RR_ENTITY_HOLE) {
        yarn->entity.status = RR_STATUS_KILLED;
    }
    else if (target_entity->type == RR_ENTITY_ENEMY) {
        rr_point_copy(&yarn->suspended_position, &target_pos);
        rr_entity_remove_from_grid(&yarn->entity, yarn->_grid);
        yarn->entity.status = RR_STATUS_SUSPENDED;
    }
    else { /* hit other non movable object. */
        rr_yarn_explode(yarn);
    }

    yarn->status_time = RR_YARN_MOVE_TIME;
}

void rr_yarn_update_dying(rrYarn* yarn, int time) {
    rr_animation_player_update(&yarn->explode_animation, time);

    if (rr_animation_player_complete(&yarn->explode_animation))
        yarn->entity.status = RR_STATUS_KILLED;
}

void rr_yarn_update_suspended(rrYarn* yarn, int time) {
    rrEntity* target_entity = rr_grid_get_entity_at_position(yarn->_grid, &yarn->suspended_position);
    rrPoint target_pos;

    /* if the grid cell is no longer occupied, we can place the yarn there and resume normal operations */
    if (target_entity == NULL) {
        rr_entity_place_in_grid_cell(&yarn->entity, yarn->_grid, &yarn->suspended_position);
        yarn->entity.status = RR_STATUS_ACTIVE;
        rr_yarn_update_active(yarn, time);
        return;
    }

    yarn->status_time += time;

    if (yarn->status_time < RR_YARN_MOVE_TIME)
        return;

    /* while we are suspended we can only move to a free cell or one that is occupied by an enemy */
    rr_point_add(&target_pos, &yarn->suspended_position, &yarn->direction);
    target_entity = rr_grid_get_entity_at_position(yarn->_grid, &target_pos);

    if (target_entity == NULL || target_entity->type == RR_ENTITY_ENEMY)
        rr_point_copy(&yarn->suspended_position, &target_pos);
    else
        yarn->entity.status = RR_STATUS_DYING;

}

void rr_yarn_update(rrYarn* yarn, int time) {
    switch (yarn->entity.status) {
        case RR_STATUS_WAITING:
            rr_yarn_update_waiting(yarn, time);
            break;

        case RR_STATUS_ACTIVE:
            rr_yarn_update_active(yarn, time);
            break;

        case RR_STATUS_DYING:
            rr_yarn_update_dying(yarn, time);
            break;

        case RR_STATUS_SUSPENDED:
            rr_yarn_update_suspended(yarn, time);
            break;

        default:
            break;
    }
}

void _rr_yarn_reset(rrYarn* yarn) {
    yarn->move_count = 0;
    yarn->status_time = 0;
    rr_point_set(&yarn->direction, 0, 0);
    rr_point_set(&yarn->suspended_position, 0, 0);
}

void* _rr_yarn_create_pooled(void* user_data){
    rrGame* game = (rrGame*)user_data;
    rrYarn* yarn = malloc(sizeof(rrYarn));
    rr_yarn_init(yarn, rr_game_get_grid(game), rr_game_get_renderer(game));

    return yarn;
}

void _rr_yarn_reset_pooled(void* item, void* user_data) {
    rrYarn* yarn = (rrYarn*)item;
    rrGame* game = (rrGame*)user_data;

    _rr_yarn_reset(yarn);
    if (!rr_entity_position_is_invalid(&yarn->entity))
        rr_entity_remove_from_grid(&yarn->entity, rr_game_get_grid(game));
}
