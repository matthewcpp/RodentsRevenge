#include "yarn.h"

#include "game.h"

#include <assert.h>

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
    yarn->entity.status = RR_STATUS_ACTIVE;
}

void rr_yarn_update(rrYarn* yarn) {
    (void)yarn;
}

void _rr_yarn_reset(rrYarn* yarn) {
    yarn->_collision = NULL;
    yarn->first_move = 1;
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
