#ifndef RR_ANIMATION_H
#define RR_ANIMATION_H

#include "../point.h"
#include "spritesheet.h"

typedef struct {
    int frame_count;
    rrSprite** frames;
    int frame_time;

} rrAnimation;

typedef struct {
    rrAnimation* animation;
    int frame_index;
    int current_time;
} rrAnimationPlayer;

rrAnimation* rr_animation_create(int frame_count, rrSprite** frames, int frame_time);
void rr_animation_destroy(rrAnimation* animation);
rrSprite* rr_animation_player_get_current_sprite(rrAnimationPlayer* animation);

void rr_animation_player_init(rrAnimationPlayer* animation_player, rrAnimation* animation);
void rr_animation_player_update(rrAnimationPlayer* animation, int time);
void rr_animation_player_reset(rrAnimationPlayer* animation);
int rr_animation_player_complete(rrAnimationPlayer* animation);

#endif
