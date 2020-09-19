#include "vec2.h"

#include <math.h>

void rr_vec2_from_point(rrVec2* vec, rrPoint* pt) {
    vec->x = (float)pt->x;
    vec->y = (float)pt->y;
}

void rr_vec2_to_point(rrPoint* pt, rrVec2* vec) {
    pt->x = (int) vec->x;
    pt->y = (int) vec->y;
}

void rr_vec2_copy(rrVec2* dest, rrVec2* src) {
    dest->x = src->x;
    dest->y = src->y;
}

float rr_vec2_length(rrVec2* vec) {
    return (float)sqrt(vec->x * vec->x + vec->y * vec->y);
}

void rr_vec2_sub(rrVec2* dest, rrVec2* a, rrVec2* b) {
    dest->x = a->x - b->x;
    dest->y = a->y - b->y;
}

void rr_vec2_add(rrVec2* dest, rrVec2* a, rrVec2* b) {
    dest->x = a->x + b->x;
    dest->y = a->y + b->y;
}

void rr_vec2_scale(rrVec2* dest, rrVec2* vec, float k) {
    dest->x = vec->x * k;
    dest->y = vec->y *k;
}

void rr_vec2_normalize(rrVec2* vec) {
    float length = rr_vec2_length(vec);

    if (length > 0.0f) {
        vec->x /= length;
        vec->y /= length;
    }
}
