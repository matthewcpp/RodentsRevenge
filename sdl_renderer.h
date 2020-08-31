#ifndef RR_SDL_RENDERER_H
#define RR_SDL_RENDERER_H

#include "game.h"

#include "SDL.h"

typedef struct {
    rrGame* _game;
    SDL_Renderer* _renderer;
    SDL_Rect _sprites[RR_CELL_COUNT];
    SDL_Texture * _spritesheet;
} rrSDLRenderer;

void rr_sdl_renderer_init(rrSDLRenderer* renderer, rrGame* game, SDL_Window* window);
int rr_sdl_renderer_load_spritesheet(rrSDLRenderer* renderer, const char* path);
void rr_sdl_renderer_draw(rrSDLRenderer* renderer);

#endif
