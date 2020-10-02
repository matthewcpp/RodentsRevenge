#ifndef RR_COLOR_H
#define RR_COLOR_H

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} rrColor;

void rr_color_set(rrColor* color, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void rr_color_black(rrColor* color);
void rr_color_white(rrColor* color);
void rr_color_blue(rrColor* color);
void rr_color_red(rrColor* color);

#endif
