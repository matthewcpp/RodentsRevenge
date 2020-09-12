#include "../animation.h"
#include "sdl_animation.h"

#include <SDL.h>

#include <stdlib.h>
#include <string.h>

rrAnimation* rr_sdl_animation_create(SDL_Texture* texture, int frame_count, rrPoint* frames, rrPoint* frame_size, int frame_time) {
    int frame_data_size = frame_count * sizeof(rrPoint);
    rrAnimation* animation = malloc(sizeof(rrAnimation));

    animation->texture = texture;
    animation->frame_count = frame_count;
    animation->frame_time = frame_time;
    animation->frames = malloc(frame_data_size);
    memcpy(animation->frames, frames, frame_data_size);

    animation->current_frame_rect.w = frame_size->x;
    animation->current_frame_rect.h = frame_size->y;

    rr_animation_reset(animation);

    return animation;
}

void rr_sdk_animation_update_frame_rect(rrAnimation* animation) {
    int frame_index = animation->current_time / animation->frame_time;

    animation->current_frame_rect.x = animation->frames[frame_index].x;
    animation->current_frame_rect.y = animation->frames[frame_index].y;
}

void rr_sdl_animation_destroy(rrAnimation* animation) {
    free(animation->frames);
    free(animation);
}

void rr_animation_update(rrAnimation* animation, int time) {
    if (animation->current_time < animation->frame_time * animation->frame_count){
        animation->current_time += time;
        rr_sdk_animation_update_frame_rect(animation);
    }

}

void rr_animation_reset(rrAnimation* animation) {
    animation->current_time = 0;
    rr_sdk_animation_update_frame_rect(animation);
}
int rr_animation_complete(rrAnimation* animation) {
    return animation->current_time >= animation->frame_time * animation->frame_count;
}
