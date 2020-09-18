#ifndef RR_VEC2_H
#define RR_VEC2_H

#include "point.h"

typedef struct {
    float x;
    float y;
} rrVec2;

void rr_vec2_from_point(rrVec2* vec, rrPoint* pt);
void rr_vec2_to_point(rrPoint* pt, rrVec2* vec);
void rr_vec2_copy(rrVec2* dest, rrVec2* src);
void rr_vec2_sub(rrVec2* dest, rrVec2* a, rrVec2* b);
void rr_vec2_add(rrVec2* dest, rrVec2* a, rrVec2* b);
void rr_vec2_scale(rrVec2* dest, rrVec2* vec, float k);

float rr_vec2_length(rrVec2* vec);
void rr_vec2_normalize(rrVec2* vec);

#endif
