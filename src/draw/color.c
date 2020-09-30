#include "color.h"

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
