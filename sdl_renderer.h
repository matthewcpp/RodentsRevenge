#ifndef RR_SDL_RENDERER_H
#define RR_SDL_RENDERER_H

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
} rrSDLRenderer;

void rr_sdl_renderer_init(SDL_Window* window, rrSDLRenderer* renderer, rrGame* game);
void rr_sdl_renderer_uninit(rrSDLRenderer* renderer);
int rr_sdl_renderer_load_spritesheet(rrSDLRenderer* renderer, const char* path);
int rr_sdl_renderer_load_font(rrSDLRenderer* renderer, const char* path);
void rr_sdl_renderer_draw(rrSDLRenderer* renderer);

#endif
