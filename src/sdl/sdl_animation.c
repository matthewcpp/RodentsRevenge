#include "../animation.h"
#include "sdl_animation.h"

#include <stdlib.h>
#include <string.h>

rrAnimation* rr_sdl_animation_create(rrSpritesheet* spritesheet, int frame_count, rrSprite** frames, int frame_time) {
    rrAnimation* animation = malloc(sizeof(rrAnimation));

    animation->frame_index = 0;
    animation->spritesheet = spritesheet;
    animation->frame_count = frame_count;
    animation->frame_time = frame_time;
    animation->frames = calloc(frame_count, sizeof(rrSprite*));
    memcpy(animation->frames, frames, frame_count * sizeof(rrSprite*));

    rr_animation_reset(animation);

    return animation;
}

rrSprite* rr_sdl_animation_get_current_sprite(rrAnimation* animation) {
    return animation->frames[animation->frame_index];
}

void rr_sdl_animation_destroy(rrAnimation* animation) {
    free(animation->frames);
    free(animation);
}

void rr_animation_update(rrAnimation* animation, int time) {
    animation->current_time += time;

    if (animation->current_time >= animation->frame_time){
        animation->frame_index += animation->frame_index < animation->frame_count -1 ? 1 : 0;
        animation->current_time -= animation->frame_time;
    }
}

void rr_animation_reset(rrAnimation* animation) {
    animation->current_time = 0;
    animation->frame_index = 0;
}
int rr_animation_complete(rrAnimation* animation) {
    return animation->frame_index == animation->frame_count - 1;
}
