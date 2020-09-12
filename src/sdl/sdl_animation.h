#ifndef RR_SDL_ANIMATION_H
#define RR_SDL_ANIMATION_H

#include "animation.h"
#include "point.h"

#include <SDL.h>

rrAnimation* rr_sdl_animation_create(SDL_Texture* texture, int frame_count, rrPoint* frames, rrPoint* frame_size, int frame_time);
void rr_sdl_animation_destroy(rrAnimation* animation);

#endif
