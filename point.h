#ifndef RR_POINT_H
#define RR_POINT_H

typedef struct {
    int x;
    int y;
} rrPoint;

void rr_point_set(rrPoint* point, int x, int y);
void rr_point_copy(rrPoint* dest, rrPoint* src);
void rr_point_add(rrPoint* out, rrPoint* a, rrPoint* b);
void rr_point_sub(rrPoint* out, rrPoint* a, rrPoint* b);

#endif
