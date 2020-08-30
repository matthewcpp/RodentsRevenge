#include "sdl_controller.h"

#include "stdio.h"

void rr_sdl_controller_init(rrSDLController* controller, rrGame* game) {
    controller->_joystick = SDL_JoystickOpen(0);
    controller->_game = game;
}

void rr_sdl_controller_update(rrSDLController* controller) {
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