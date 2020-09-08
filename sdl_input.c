#include "sdl_input.h"

#include <stdio.h>

rrInput* rr_sdl_input_create(int joystick_index) {
    rrInput* input = malloc(sizeof(rrInput));
    input->_joystick = SDL_JoystickOpen(joystick_index);
    memset(input->_button_info, 0, sizeof(rrSDLButtonInfo) * RR_INPUT_BUTTON_COUNT);

    return input;
}

int rr_sdl_input_joystick_active(rrInput* input) {
    return input->_joystick != NULL;
}

void rr_sdl_input_destroy(rrInput* input) {
    if (input->_joystick)
        SDL_JoystickClose(input->_joystick);

    free(input);
}

int rr_input_button_down(rrInput* input, rrInputButton button) {
    return input->_button_info[button].prev_state == 0 && input->_button_info[button].cur_state == 1;
}

int rr_input_button_held_time(rrInput* input, rrInputButton button) {
    return input->_button_info[button].hold_time;
}

void rr_sdl_input_update_button(rrInput* input, rrInputButton button, int value, int time) {
    rrSDLButtonInfo* button_info = &input->_button_info[button];
    button_info->prev_state = button_info->cur_state;
    button_info->cur_state = value;

    if (button_info->prev_state && button_info->cur_state)
        button_info->hold_time += time;
    else
        button_info->hold_time = 0;
}

void rr_sdl_input_update_joystick(rrInput* input, int time) {
    Uint8 hat = SDL_JoystickGetHat(input->_joystick, 0);

    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_UP, hat & SDL_HAT_UP, time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_DOWN, hat & SDL_HAT_DOWN, time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_LEFT, hat & SDL_HAT_LEFT, time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_RIGHT, hat & SDL_HAT_RIGHT, time);
}

void rr_sdl_update_keyboard(rrInput* input, int time) {
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_UP, keyboard_state[SDL_SCANCODE_UP], time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_DOWN, keyboard_state[SDL_SCANCODE_DOWN], time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_LEFT, keyboard_state[SDL_SCANCODE_LEFT], time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_RIGHT, keyboard_state[SDL_SCANCODE_RIGHT], time);
}

void rr_sdl_input_update(rrInput* input, int time) {
    if (input->_joystick)
        rr_sdl_input_update_joystick(input, time);
    else
        rr_sdl_update_keyboard(input, time);
}
