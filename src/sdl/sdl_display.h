#ifndef rr_sdl_display_H
#define rr_sdl_display_H

#include "../game.h"

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct rrSDLDisplay rrSDLDisplay;

rrSDLDisplay* rr_sdl_display_create(SDL_Window* window, rrGame* game, rrInput* input);
void rr_sdl_display_destroy(rrSDLDisplay* display);
int rr_sdl_display_load_sprites(rrSDLDisplay* display, const char* asset_dir);
int rr_sdl_display_load_fonts(rrSDLDisplay* display, const char* asset_dir);
void rr_sdl_display_draw(rrSDLDisplay* display);

#endif
