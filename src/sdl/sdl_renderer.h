#ifndef RR_SDL_RENDERER_H
#define RR_SDL_RENDERER_H

#include "../draw/renderer.h"

#include <SDL.h>
#include <SDL_ttf.h>

rrRenderer* rr_sdl_renderer_create(SDL_Renderer* renderer);

/* TODO: Implement me */
void rr_sdl_renderer_destroy(rrRenderer* renderer);

void rr_sdl_renderer_set_screen_size(rrRenderer* renderer, rrPoint* screen_size);

/* TODO: replace with font loading code */
void rr_sdl_renderer_set_font(rrRenderer* renderer, TTF_Font* font);

#endif
