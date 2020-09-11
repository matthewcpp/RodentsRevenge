#ifndef RR_ANIMATION_H
#define RR_ANIMATION_H

#include "point.h"

typedef struct rrAnimation rrAnimation;

void rr_animation_update(rrAnimation* animation, int time);
void rr_animation_reset(rrAnimation* animation);
int rr_animation_complete(rrAnimation* animation);

#endif
