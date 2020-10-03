#include "sdl_game.h"

#include "sdl_input.h"

#include <SDL.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

rrSDLGame* rr_sdl_game_create(const char* asset_path) {
    rrSDLGame* game = calloc(sizeof(rrSDLGame), 1);

    game->error_str = NULL;
    game->asset_path = malloc(strlen(asset_path) + 1);
    strcpy(game->asset_path, asset_path);

    return game;
}

int rr_sdl_game_init(rrSDLGame* game, int screen_width, int screen_height) {
    int result;

    srand ((unsigned int)time(NULL));

    SDL_VideoInit(NULL);
    game->window = SDL_CreateWindow("RodentsRevenge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height,SDL_WINDOW_SHOWN);

    if (!game->window) {
        game->error_str = "Failed to create SDL window";
        return 0;
    }

    result = SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    if (result != 0) {
        game->error_str = "Failed to initialize controller subsystem";
        return 0;
    }

    game->input = rr_sdl_input_create(0, NULL);

    if (!game->input) {
        game->error_str = "Failed to open game controller";
        return 0;
    }

    game->renderer = rr_sdl_renderer_create(game->window);
    if (!rr_sdl_renderer_load_sprites(game->renderer, game->asset_path)) {
        game->error_str = "Failed to load sprites";
        return 0;
    }

    if (!rr_sdl_renderer_load_fonts(game->renderer, game->asset_path)) {
        game->error_str = "Failed to load fonts";
        return 0;
    }

    game->game = rr_game_create(game->input, game->renderer, game->asset_path);
    game->high_scores = rr_high_scores_create();
    rr_high_scores_load_file(game->high_scores, "/Users/matthew/development/scratch/high_scores.txt");
    game->display = rr_sdl_display_create(game->game, game->high_scores, game->input, game->renderer);

    rr_sdl_display_init_ui(game->display);

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

            rr_sdl_renderer_begin(game->renderer);
            rr_sdl_display_draw(game->display);
            rr_sdl_renderer_end(game->renderer);

            last_update = now;
        }
        SDL_Delay(1);
    }
}

void rr_sdl_game_destroy(rrSDLGame* game) {
    if (game->input)
        rr_sdl_input_destroy(game->input);

    if (game->display)
        rr_sdl_display_destroy(game->display);

    if (game->asset_path)
        free(game->asset_path);

    if (game->game) {
        rr_game_destroy(game->game);
    }

    if (game->high_scores)
        rr_high_scores_destroy(game->high_scores);

    if (game->renderer)
        rr_sdl_renderer_destroy(game->renderer);

    if (game->window) {
        SDL_DestroyWindow(game->window);
    }

    SDL_VideoQuit();
}

const char* rr_sdl_game_get_error_str(rrSDLGame* game) {
    return game->error_str;
}

rrInput* rr_sdl_game_get_input(rrSDLGame* game) {
    return game->input;
}

rrGame* rr_sdl_game_get_base_game(rrSDLGame* sdl_game) {
    return sdl_game->game;
}

