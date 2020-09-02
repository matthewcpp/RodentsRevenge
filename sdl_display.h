#ifndef rr_sdl_display_H
#define rr_sdl_display_H

#include "game.h"

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct {
    rrGame* _game;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Rect _sprites[RR_CELL_COUNT];
    SDL_Texture* _spritesheet;
    TTF_Font* _font;
    SDL_Texture* _scoreText;
    SDL_Rect _scoreTextRect;
} rrSDLDisplay;

void rr_sdl_display_init(SDL_Window* window, rrSDLDisplay* renderer, rrGame* game);
void rr_sdl_display_uninit(rrSDLDisplay* renderer);
int rr_sdl_display_load_spritesheet(rrSDLDisplay* renderer, const char* path);
int rr_sdl_display_load_font(rrSDLDisplay* renderer, const char* path);
void rr_sdl_display_draw(rrSDLDisplay* renderer);

#endif
