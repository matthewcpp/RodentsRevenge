#ifndef RR_SDL_RENDERER_H
#define RR_SDL_RENDERER_H

#include "../draw/renderer.h"
#include "../draw/spritesheet.h"
#include "../assets.h"

#include "cutil/vector.h"

#include <SDL.h>
#include <SDL_ttf.h>

struct rrRenderer{
    SDL_Renderer* renderer;
    rrPoint screen_size;

    TTF_Font** fonts;
    cutil_vector* text_sprites;

    rrSprite** sprites;
    rrAnimation** animations;
    rrSpritesheet* spritesheet;
};

rrRenderer* rr_sdl_renderer_create(SDL_Window* window);
void rr_sdl_renderer_destroy(rrRenderer* renderer);
void rr_sdl_renderer_begin(rrRenderer* renderer);
void rr_sdl_renderer_end(rrRenderer* renderer);

int rr_sdl_renderer_load_sprites(rrRenderer* renderer, const char* asset_dir);
int rr_sdl_renderer_load_fonts(rrRenderer* renderer, const char* asset_dir);

#endif
