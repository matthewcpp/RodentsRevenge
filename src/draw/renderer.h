#ifndef RR_RENDERER_H
#define RR_RENDERER_H

#include "../point.h"

typedef struct rrRenderer rrRenderer;

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

typedef struct
{
    int x, y;
    int w, h;
} rrRect;

typedef struct {
    void* handle;
    rrRect rect;
} rrSprite;

void rr_renderer_get_screen_size(rrRenderer*, rrPoint* size);

void rr_renderer_color(struct rrRenderer* renderer, rrColor* color);
void rr_renderer_fill_rect(rrRenderer* renderer, rrRect* rect);
void rr_renderer_draw_rect(rrRenderer* renderer, rrRect* rect);
void rr_renderer_draw_sprite(rrRenderer* renderer, rrSprite* sprite, rrPoint* position);
void rr_renderer_draw_line(rrRenderer* renderer, rrPoint* a, rrPoint* b);

rrSprite* rr_renderer_load_sprite(rrRenderer* renderer, const char* path);

rrSprite* rr_renderer_create_text(rrRenderer* renderer, int font, const char* text);
void rr_renderer_destroy_sprite(struct rrRenderer* renderer, rrSprite* sprite);

void rr_renderer_set_sprite_tint_color(rrRenderer* renderer, rrSprite* sprite, rrColor* color);

#endif
