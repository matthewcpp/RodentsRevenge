#ifndef RR_SDL_ANIMATION_H
#define RR_SDL_ANIMATION_H

#include "../animation.h"
#include "../point.h"
#include "../draw/spritesheet.h"

#include <SDL.h>

struct rrAnimation {
    rrSpritesheet* spritesheet;
    int frame_count;
    rrSprite** frames;
    int frame_index;

    int frame_time;
    int current_time;
};

rrAnimation* rr_sdl_animation_create(rrSpritesheet* spritesheet, int frame_count, rrSprite** frames, int frame_time);
void rr_sdl_animation_destroy(rrAnimation* animation);
rrSprite* rr_sdl_animation_get_current_sprite(rrAnimation* animation);

#endif
