#ifndef RR_SDL_INPUT_H
#define RR_SDL_INPUT_H

#include "input.h"

#include <SDL.h>

rrInput* rr_sdl_input_create(int joystick_index);
void rr_sdl_input_destroy(rrInput* input);

int rr_sdl_input_joystick_active(rrInput* input);
void rr_sdl_input_update(rrInput* input, int time);

#endif
