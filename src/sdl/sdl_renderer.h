#ifndef RR_SDL_RENDERER_H
#define RR_SDL_RENDERER_H

#include "../draw/renderer.h"
#include "../assets.h"

#include "cutil/vector.h"

#include <SDL.h>
#include <SDL_ttf.h>

struct rrRenderer{
    SDL_Renderer* renderer;
    rrPoint screen_size;

    TTF_Font* font;
    cutil_vector* text_sprites;

    rrSprite** sprites;
};

rrRenderer* rr_sdl_renderer_create(SDL_Renderer* renderer);

void rr_sdl_renderer_destroy(rrRenderer* renderer);

void rr_sdl_renderer_set_screen_size(rrRenderer* renderer, rrPoint* screen_size);

/* TODO: replace with font loading code */
void rr_sdl_renderer_set_font(rrRenderer* renderer, TTF_Font* font);

#endif
