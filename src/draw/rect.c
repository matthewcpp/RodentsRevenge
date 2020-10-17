#include "rect.h"

int rr_rect_right(rrRect* rect) {
    return rect->x + rect->w;
}

int rr_rect_bottom(rrRect* rect) {
    return rect->y + rect->h;
}

void rr_rect_encapsulate(rrRect* target, rrRect* rect) {
    int target_right = rr_rect_right(target);
    int target_bottom = rr_rect_bottom(target);
    int rect_right = rr_rect_right(rect);
    int rect_bottom = rr_rect_bottom(rect);

    if (rect->x < target->x)
        target->x = rect->x;

    if (rect->y < target->y)
        target->y = rect->y;

    if (rect_right > target_right)
        target->w += rect_right - target_right;

    if (rect_bottom > target_bottom)
        target->h += rect_bottom - target_bottom;
}

int rr_rect_contains_point(rrRect* rect, int x, int y) {
    if (x < rect->x || x > rect->x + rect->w)
        return 0;

    if (y < rect->y || y > rect->y + rect->h)
        return 0;

    return 1;
}

void rr_rect_set(rrRect* rect, int x, int y, int w, int h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

void rr_rect_set_size(rrRect* rect, int w, int h) {
    rect->w = w;
    rect->h = h;
}
