#include "sdl_controller.h"

#include "stdio.h"

void rr_sdl_controller_init(rrSDLController* controller, rrGame* game) {
    controller->_joystick = SDL_JoystickOpen(0);
    controller->_game = game;
}

void rr_sdl_update_joystick(rrSDLController* controller) {
    Uint8 hat = SDL_JoystickGetHat(controller->_joystick, 0);

    if (hat & SDL_HAT_RIGHT) {
        rr_player_move(&controller->_game->player, 1, 0);
    }

    if (hat & SDL_HAT_LEFT) {
        rr_player_move(&controller->_game->player, -1, 0);
    }

    if (hat & SDL_HAT_UP) {
        rr_player_move(&controller->_game->player, 0, -1);
    }

    if (hat & SDL_HAT_DOWN) {
        rr_player_move(&controller->_game->player, 0, 1);
    }
}

void rr_sdl_update_keyboard(rrSDLController* controller) {
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    if (keyboard_state[SDL_SCANCODE_RIGHT]) {
        rr_player_move(&controller->_game->player, 1, 0);
    }

    if (keyboard_state[SDL_SCANCODE_LEFT]) {
        rr_player_move(&controller->_game->player, -1, 0);
    }

    if (keyboard_state[SDL_SCANCODE_UP]) {
        rr_player_move(&controller->_game->player, 0, -1);
    }

    if (keyboard_state[SDL_SCANCODE_DOWN]) {
        rr_player_move(&controller->_game->player, 0, 1);
    }
}

void rr_sdl_controller_update(rrSDLController* controller) {
    rr_sdl_update_joystick(controller);
    rr_sdl_update_keyboard(controller);
}