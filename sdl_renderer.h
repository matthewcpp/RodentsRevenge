#ifndef RR_SDL_RENDERER_H
#define RR_SDL_RENDERER_H

#include "game.h"

#include "SDL.h"

typedef struct {
    rrGame* _game;
    SDL_Renderer* _renderer;

} rrSDLRenderer;

void rr_sdl_renderer_init(rrSDLRenderer* renderer, rrGame* game, SDL_Window* window);
void rr_sdl_renderer_draw(rrSDLRenderer* renderer);

#endif
