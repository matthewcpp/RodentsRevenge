#include "animation.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

rrAnimation* rr_animation_create(int frame_count, rrSprite** frames, int frame_time) {
    rrAnimation* animation = malloc(sizeof(rrAnimation));

    animation->frame_count = frame_count;
    animation->frame_time = frame_time;
    animation->frames = calloc(frame_count, sizeof(rrSprite*));
    memcpy(animation->frames, frames, frame_count * sizeof(rrSprite*));

    return animation;
}

void rr_animation_destroy(rrAnimation* animation) {
    free(animation->frames);
    free(animation);
}

void rr_animation_player_init(rrAnimationPlayer* animation_player, rrAnimation* animation) {
    assert(animation != NULL);
    animation_player->animation = animation;
    rr_animation_player_reset(animation_player);
}

rrSprite* rr_animation_player_get_current_sprite(rrAnimationPlayer* animation_player) {
    return animation_player->animation->frames[animation_player->frame_index];
}

void rr_animation_player_update(rrAnimationPlayer* animation_player, int time) {
    animation_player->current_time += time;

    if (animation_player->current_time >= animation_player->animation->frame_time){
        animation_player->frame_index += animation_player->frame_index < animation_player->animation->frame_count -1 ? 1 : 0;
        animation_player->current_time -= animation_player->animation->frame_time;
    }
}

void rr_animation_player_reset(rrAnimationPlayer* animation_player) {
    animation_player->current_time = 0;
    animation_player->frame_index = 0;
}
int rr_animation_player_complete(rrAnimationPlayer* animation_player) {
    return animation_player->frame_index == animation_player->animation->frame_count - 1;
}
