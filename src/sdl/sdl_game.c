#include "sdl_game.h"

#include "sdl_input.h"
#include "sdl_display.h"

#include "../game.h"
#include "../defs.h"

#include <SDL.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

struct rrSDLGame {
    rrGame* game;
    rrInput* input;
    rrSDLDisplay* display;
    SDL_Window* window;
    char* asset_path;
    const char* error_str;
};

rrSDLGame* rr_sdl_game_create(const char* asset_path) {
    rrSDLGame* game = calloc(sizeof(rrSDLGame), 1);

    game->error_str = NULL;
    game->asset_path = malloc(strlen(asset_path) + 1);
    strcpy(game->asset_path, asset_path);

    return game;
}

int rr_sdl_game_init(rrSDLGame* game, int screen_width, int screen_height) {
    int result;
    char asset_path[256];

    srand ((unsigned int)time(NULL));

    SDL_VideoInit(NULL);
    game->window = SDL_CreateWindow("RodentsRevenge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height,SDL_WINDOW_SHOWN);

    if (!game->window) {
        game->error_str = "Failed to create SDL window";
        return 0;
    }

    result = SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    /* TODO: Xbox only */
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    if (result != 0) {
        game->error_str = "Failed to initialize controller subsystem";
        return 0;
    }

    game->input = rr_sdl_input_create(0, NULL);

    if (!game->input) {
        game->error_str = "Failed to open game controller";
        return 0;
    }

    game->game = rr_game_create(game->input, game->asset_path);
    game->display = rr_sdl_display_create(game->window, game->game);

    snprintf_func(asset_path, 256, "%s%s", game->asset_path, "spritesheet.png");
    if (!rr_sdl_display_load_spritesheet(game->display, asset_path)) {
        game->error_str = "Failed to load spritesheet";
        return 0;
    }

    snprintf_func(asset_path, 256, "%s%s", game->asset_path, "vegur-regular.ttf");
    if (!rr_sdl_display_load_font(game->display, asset_path)) {
        game->error_str = "Failed to load font";
        return 0;
    }

    return 1;
}

void rr_sdl_game_run(rrSDLGame* game) {
    SDL_Event event;
    int keep_going = 1;
    Uint32 last_update, now, time_delta;

    last_update = SDL_GetTicks();

    while (keep_going) {
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    keep_going = 0;
                    break;
            }
        }

        now = SDL_GetTicks();
        time_delta = now - last_update;
        if (time_delta >= 32) {
            rr_sdl_input_update(game->input, time_delta);
            rr_game_update(game->game, time_delta);
            rr_sdl_display_draw(game->display);
            last_update = now;
        }
        SDL_Delay(1);
    }
}

void rr_sdl_game_destroy(rrSDLGame* game) {
    rr_sdl_input_destroy(game->input);
    rr_sdl_display_destroy(game->display);
    rr_game_destroy(game->game);

    free(game->asset_path);

    SDL_VideoQuit();
}

const char* rr_sdl_game_get_error_str(rrSDLGame* game) {
    return game->error_str;
}
