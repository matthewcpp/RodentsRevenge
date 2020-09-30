#ifndef RR_GAME_H
#define RR_GAME_H

#include "input.h"
#include "clock.h"
#include "grid.h"
#include "player.h"

typedef enum {
    RR_GAME_STATE_UNSTARTED,
    RR_GAME_STATE_PLAYING,
    RR_GAME_STATE_WINDING_CLOCK,
    RR_GAME_STATE_PAUSED
} rrGameState;

typedef struct rrGame rrGame;

rrGame* rr_game_create(rrInput* input, rrRenderer* renderer, const char* asset_path);
void rr_game_destroy(rrGame* game);

rrClock* rr_game_get_clock(rrGame* game);
rrPlayer* rr_game_get_player(rrGame* game);
rrGrid* rr_game_get_grid(rrGame* game);
rrGameState rr_game_get_state(rrGame* game);

void rr_game_set_paused(rrGame* game, int paused);

void rr_game_update(rrGame* game, int time);

void rr_game_reset(rrGame* game);
int rr_game_restart(rrGame* game);
int rr_game_set_active_level(rrGame* game, int level_num);
int rr_game_get_level_count();

#endif
