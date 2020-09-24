#include "renderer.h"

void rr_color_black(rrColor* color) {
    color->r = 0;
    color->g = 0;
    color->b = 0;
    color->a = 255;
}

void rr_color_white(rrColor* color) {
    color->r = 255;
    color->g = 255;
    color->b = 255;
    color->a = 255;
}

void rr_color_blue(rrColor* color) {
    color->r = 0;
    color->g = 0;
    color->b = 255;
    color->a = 255;
}

void rr_color_red(rrColor* color) {
    color->r = 255;
    color->g = 0;
    color->b = 0;
    color->a = 255;
}

void rr_color_set(rrColor*color, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}

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
