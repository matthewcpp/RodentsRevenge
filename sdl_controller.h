#ifndef RR_CONTROLLER_H
#define RR_CONTROLLER_H

#include "game.h"

#include "SDL.h"

typedef struct {
    rrGame* _game;
    SDL_Joystick* _joystick;
} rrSDLController;

void rr_sdl_controller_init(rrSDLController* controller, rrGame* game);

void rr_sdl_controller_update(rrSDLController* controller);


#endif
