#ifndef RR_SDL_GAME_H
#define RR_SDL_GAME_H

#include "../input.h"
#include "../game.h"
#include "../high_scores.h"

#include "sdl_renderer.h"
#include "sdl_display.h"

typedef struct {
    rrGame* game;
    rrHighScores* high_scores;
    rrInput* input;
    rrSDLDisplay* display;
    struct rrRenderer* renderer;

    SDL_Window* window;
    char* asset_path;
    char* data_path;
    const char* error_str;
} rrSDLGame;

rrSDLGame* rr_sdl_game_create(const char* asset_path, const char* data_path);
int rr_sdl_game_init(rrSDLGame* game, int screen_width, int screen_height);
rrInput* rr_sdl_game_get_input(rrSDLGame* game);
void rr_sdl_game_run(rrSDLGame* game);
void rr_sdl_game_destroy(rrSDLGame* game);
const char* rr_sdl_game_get_error_str(rrSDLGame* game);

/** Gets the platform independent game object. */
rrGame* rr_sdl_game_get_base_game(rrSDLGame* sdl_game);

#endif
