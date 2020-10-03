#ifndef rr_sdl_display_H
#define rr_sdl_display_H

#include "../game.h"
#include "../high_scores.h"

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct rrSDLDisplay rrSDLDisplay;

rrSDLDisplay* rr_sdl_display_create(rrGame* game, rrHighScores* high_scores, rrInput* input, rrRenderer* sdl_renderer);
void rr_sdl_display_init_ui(rrSDLDisplay* display);
void rr_sdl_display_destroy(rrSDLDisplay* display);
void rr_sdl_display_draw(rrSDLDisplay* display);

#endif
