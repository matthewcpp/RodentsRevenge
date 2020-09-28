#include "yarn.h"

#include "game.h"

void* _rr_yarn_create_pooled(void* user_data){
    rrGame* game = (rrGame*)user_data;
    rrYarn* yarn = malloc(sizeof(rrYarn));
    rr_yarn_init(yarn, rr_game_get_grid(game));

    return yarn;
}

void _rr_yarn_reset_pooled(void* item, void* user_data) {
    (void)item;
    (void)user_data;
}

void rr_yarn_init(rrYarn* yarn, rrGrid* grid) {
    rr_entity_init(&yarn->entity, RR_ENTITY_YARN);
    yarn->_grid = grid;
}

void rr_yarn_update(rrYarn* yarn) {
    (void)yarn;
}
