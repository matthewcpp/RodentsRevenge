#include "defs.h"
#include "game.h"

#include <SDL.h>
#include "sdl_controller.h"
#include "sdl_display.h"

#include <stdlib.h>
#include <stdio.h>

#define RR_SCREEN_WIDTH 640
#define RR_SCREEN_HEIGHT 480

int main(int argc, char* argv[]){
    SDL_Window* window = NULL;
    SDL_Event event;
    int keep_going = 1;
    Uint32 last_update, now, time_delta;
    char asset_path[256];

    rrGame* game = NULL;
    rrSDLController controller;
    rrSDLDisplay renderer;
    (void)argc;
    (void)argv;

    game = malloc(sizeof(rrGame));

    SDL_VideoInit(NULL);
    window = SDL_CreateWindow("RodentsRevenge",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              RR_SCREEN_WIDTH,
                              RR_SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    if (SDL_NumJoysticks() < 1) {
        puts("No joysticks detected");
    }

    rr_sdl_controller_init(&controller, game);
    rr_sdl_display_init(window, &renderer, game);

    rr_game_init(game);

    snprintf_func(asset_path, 256, "%s/%s", ASSET_DIRECTORY, "spritesheet.png");
    rr_sdl_display_load_spritesheet(&renderer, asset_path);

    snprintf_func(asset_path, 256, "%s/%s", ASSET_DIRECTORY, "vegur-regular.ttf");
    rr_sdl_display_load_font(&renderer, asset_path);

    snprintf_func(asset_path, 256, "%s/%s", ASSET_DIRECTORY, "levels/level01.txt");
    rr_game_set_active_level(game, asset_path);

    rr_game_restart(game);

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
            rr_sdl_controller_update(&controller);
            rr_game_update(game, time_delta);
            rr_sdl_display_draw(&renderer);
            last_update = now;
        }
        SDL_Delay(1);
    }

    rr_game_uninit(game);
    free(game);
    rr_sdl_display_uninit(&renderer);
    SDL_VideoQuit();

    return 0;
}
