#ifndef RR_SDL_INPUT_H
#define RR_SDL_INPUT_H

#include "input.h"

#include <SDL.h>

typedef struct {
    int prev_state;
    int cur_state;
    int hold_time;
} rrSDLButtonInfo;

struct rrInput{
    SDL_Joystick* _joystick;
    rrSDLButtonInfo _button_info[RR_INPUT_BUTTON_COUNT];
};

rrInput* rr_sdl_input_create(int joystick_index);
int rr_sdl_input_joystick_active(rrInput* input);
void rr_sdl_input_destroy(rrInput* input);
void rr_sdl_input_update(rrInput* input, int time);

#endif
