#include "point.h"

void rr_point_set(rrPoint* point, int x, int y) {
    point->x = x;
    point->y = y;
}

void rr_point_copy(rrPoint* dest, rrPoint* src) {
    dest->x = src->x;
    dest->y = src->y;
}

void rr_point_add(rrPoint* out, rrPoint* a, rrPoint* b) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
}

void rr_point_sub(rrPoint* out, rrPoint* a, rrPoint* b) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
}

int rr_point_dist_squared(rrPoint* a, rrPoint* b) {
    rrPoint dir;
    rr_point_sub(&dir, a, b);

    return dir.x * dir.x + dir.y * dir.y;
}

int rr_point_equals(rrPoint* a, rrPoint* b) {
    return a->x == b->x && a->y == b->y;
}

rrPoint _rr_point_zero = {0, 0};

rrPoint* rr_point_zero() {
    return &_rr_point_zero;
}
