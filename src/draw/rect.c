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
