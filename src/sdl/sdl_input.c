#include "sdl_input.h"

#include <string.h>
#include <stdlib.h>

typedef struct {
    int prev_state;
    int cur_state;
    int hold_time;
} rrSDLButtonInfo;

typedef struct {
    int prev_state;
    int cur_state;
    rrPoint position;
} rrSDLPointerInfo;

struct rrInput{
    SDL_GameController* controller;
    rrSDLButtonInfo _button_info[RR_INPUT_BUTTON_COUNT];
    rrSDLPointerInfo _pointer;
};

rrInput* rr_sdl_input_create(int joystick_index, const char* mapping_file_path) {
    rrInput* input = NULL;
    SDL_GameController* controller = SDL_GameControllerOpen(joystick_index);

    input = malloc(sizeof(rrInput));

    if (mapping_file_path) {
        SDL_GameControllerAddMappingsFromFile(mapping_file_path);
    }

    input->controller = controller;
    memset(input->_button_info, 0, sizeof(rrSDLButtonInfo) * RR_INPUT_BUTTON_COUNT);
    memset(&input->_pointer, 0, sizeof(rrSDLPointerInfo));

    return input;
}

int rr_sdl_input_controller_active(rrInput* input) {
    return input->controller != NULL;
}

void rr_sdl_input_destroy(rrInput* input) {
    if (input->controller)
        SDL_GameControllerClose(input->controller);

    free(input);
}

int rr_input_button_down(rrInput* input, rrInputButton button) {
    return input->_button_info[button].prev_state == 0 && input->_button_info[button].cur_state == 1;
}

int rr_input_button_held_time(rrInput* input, rrInputButton button) {
    return input->_button_info[button].hold_time;
}

int rr_input_pointer_down(rrInput* input){
    return input->_pointer.cur_state == 0 && input->_pointer.prev_state == 1;
}

int rr_input_pointer_up(rrInput* input){
    return input->_pointer.cur_state == 1 && input->_pointer.prev_state == 0;
}

void rr_input_pointer_pos(rrInput* input, rrPoint* pos) {
    rr_point_copy(pos, &input->_pointer.position);
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
    SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_DPAD_UP );

    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_UP, SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_DPAD_UP ), time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_DOWN, SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN ), time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_LEFT, SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT ), time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_RIGHT, SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT ), time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_START, SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_START ), time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_ACCEPT, SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_A ), time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_BACK, SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_B ), time);
}

void rr_sdl_update_keyboard(rrInput* input, int time) {
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_UP, keyboard_state[SDL_SCANCODE_UP], time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_DOWN, keyboard_state[SDL_SCANCODE_DOWN], time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_LEFT, keyboard_state[SDL_SCANCODE_LEFT], time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_RIGHT, keyboard_state[SDL_SCANCODE_RIGHT], time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_START, keyboard_state[SDL_SCANCODE_SPACE], time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_ACCEPT, keyboard_state[SDL_SCANCODE_RETURN], time);
    rr_sdl_input_update_button(input, RR_INPUT_BUTTON_BACK, keyboard_state[SDL_SCANCODE_ESCAPE], time);
}

void rr_sdl_update_pointer(rrInput* input) {
    Uint32 mouse_state = SDL_GetMouseState(&input->_pointer.position.x, &input->_pointer.position.y);
    input->_pointer.prev_state = input->_pointer.cur_state;
    input->_pointer.cur_state = mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT);
}

void rr_sdl_input_update(rrInput* input, int time) {
    rr_sdl_update_pointer(input);

    if (input->controller)
        rr_sdl_input_update_joystick(input, time);
    else
        rr_sdl_update_keyboard(input, time);
}
