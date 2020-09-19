#ifndef RR_ANIMATION_H
#define RR_ANIMATION_H

#include "../point.h"
#include "spritesheet.h"

typedef struct {
    rrSpritesheet* spritesheet;
    int frame_count;
    rrSprite** frames;
    int frame_index;

    int frame_time;
    int current_time;
} rrAnimation;

rrAnimation* rr_animation_create(rrSpritesheet* spritesheet, int frame_count, rrSprite** frames, int frame_time);
void rr_animation_destroy(rrAnimation* animation);
rrSprite* rr_animation_get_current_sprite(rrAnimation* animation);

void rr_animation_update(rrAnimation* animation, int time);
void rr_animation_reset(rrAnimation* animation);
int rr_animation_complete(rrAnimation* animation);

#endif
