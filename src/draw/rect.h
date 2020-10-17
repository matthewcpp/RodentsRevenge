#ifndef RR_RECT_H
#define RR_RECT_H

typedef struct
{
    int x, y;
    int w, h;
} rrRect;

void rr_rect_encapsulate(rrRect* target, rrRect* rect);
int rr_rect_right(rrRect* rect);
int rr_rect_bottom(rrRect* rect);
int rr_rect_contains_point(rrRect* rect, int x, int y);
void rr_rect_set(rrRect* rect, int x, int y, int w, int h);
void rr_rect_set_size(rrRect* rect, int w, int h);

#endif
