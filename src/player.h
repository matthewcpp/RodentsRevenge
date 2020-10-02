#ifndef RR_PLAYER_H
#define RR_PLAYER_H

#include "entity.h"
#include "point.h"
#include "grid.h"
#include "input.h"
#include "draw/animation.h"
#include "draw/renderer.h"

#include <stdint.h>

typedef struct rrPlayer {
    rrEntity entity;
    rrGrid* _grid;
    rrInput* _input;
    rrRenderer* _renderer;
    int score;
    int lives_remaining;
    int time_stuck;

    rrAnimationPlayer death_animation;
    rrEntity* _killer_entity;
} rrPlayer;

void rr_player_init(rrPlayer* player, rrGrid* grid, rrInput* input, rrRenderer* renderer);
void rr_player_init_assets(rrPlayer* player);
void rr_player_update(rrPlayer* player, int time);
void rr_player_move(rrPlayer* player, rrPoint* delta);

/**
 * Kills the player and begins the death animation.
 * The supplied entity should have suspended status.
 * It will be returned to active duty at the player's position when the death animation has finished.
 */
void rr_player_kill(rrPlayer* player, rrEntity* entity);

#endif
