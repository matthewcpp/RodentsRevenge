#ifndef RR_RENDERER_H
#define RR_RENDERER_H

#include "../point.h"

#include "animation.h"
#include "color.h"

typedef struct rrRenderer rrRenderer;

void rr_renderer_get_screen_size(rrRenderer*, rrPoint* size);

void rr_renderer_color(struct rrRenderer* renderer, rrColor* color);
void rr_renderer_fill_rect(rrRenderer* renderer, rrRect* rect);
void rr_renderer_draw_rect(rrRenderer* renderer, rrRect* rect);
void rr_renderer_draw_sprite(rrRenderer* renderer, rrSprite* sprite, rrPoint* position);
void rr_renderer_draw_line(rrRenderer* renderer, rrPoint* a, rrPoint* b);

rrSprite* rr_renderer_load_sprite(rrRenderer* renderer, const char* path);
rrSprite* rr_renderer_create_text(rrRenderer* renderer, int font, const char* text);
rrAnimation* rr_renderer_create_animation(rrRenderer* renderer, int index, int frame_count, rrSprite** frames, int frame_time);
void rr_renderer_update_text_sprite(rrRenderer* renderer, rrSprite* text_sprite, int font, const char* text);

void rr_renderer_set_sprite_tint_color(rrRenderer* renderer, rrSprite* sprite, rrColor* color);

rrSprite* rr_renderer_get_sprite(rrRenderer* renderer, int index);
rrAnimation* rr_renderer_get_animation(rrRenderer* renderer, int index);
#endif
