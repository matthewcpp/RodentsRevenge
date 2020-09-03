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
