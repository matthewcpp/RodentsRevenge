#include "sdl_controller.h"

#include "stdio.h"

void rr_sdl_controller_init(rrSDLController* controller, rrGame* game) {
    controller->_joystick = SDL_JoystickOpen(0);
    controller->_game = game;
}

void rr_sdl_update_joystick(rrSDLController* controller) {
    Uint8 hat = SDL_JoystickGetHat(controller->_joystick, 0);
    rrPoint delta;
    rr_point_set(&delta, 0, 0);

    if (hat & SDL_HAT_RIGHT) {
        delta.x = 1;
        rr_player_move(&controller->_game->player, &delta);
    }

    if (hat & SDL_HAT_LEFT) {
        delta.x = -1;
        rr_player_move(&controller->_game->player, &delta);
    }

    if (hat & SDL_HAT_UP) {
        delta.y = -1;
        rr_player_move(&controller->_game->player, &delta);
    }

    if (hat & SDL_HAT_DOWN) {
        delta.y = 1;
        rr_player_move(&controller->_game->player, &delta);
    }
}

void rr_sdl_update_keyboard(rrSDLController* controller) {
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    rrPoint delta;
    rr_point_set(&delta, 0, 0);

    if (keyboard_state[SDL_SCANCODE_RIGHT]) {
        delta.x = 1;
        rr_player_move(&controller->_game->player, &delta);
    }

    if (keyboard_state[SDL_SCANCODE_LEFT]) {
        delta.x = -1;
        rr_player_move(&controller->_game->player, &delta);
    }

    if (keyboard_state[SDL_SCANCODE_UP]) {
        delta.y = -1;
        rr_player_move(&controller->_game->player, &delta);
    }

    if (keyboard_state[SDL_SCANCODE_DOWN]) {
        delta.y = 1;
        rr_player_move(&controller->_game->player, &delta);
    }
}

void rr_sdl_controller_update(rrSDLController* controller) {
    rr_sdl_update_joystick(controller);
    rr_sdl_update_keyboard(controller);
}
