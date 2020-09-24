#ifndef RR_BASIC_SPRITE_H
#define RR_BASIC_SPRITE_H

#include "../draw/renderer.h"
#include "element.h"

typedef struct {
    rrUiElement element;
    rrSprite* sprite;
    rrRenderer* _renderer;
} rrUiBasicSprite;

void rr_ui_basic_sprite_init(rrUiBasicSprite* basic_sprite, rrPoint* position, rrSprite* sprite, rrRenderer* renderer);
void rr_ui_basic_sprite_draw(rrUiBasicSprite* basic_sprite);
void rr_ui_basic_sprite_get_rect(rrUiBasicSprite* basic_sprite, rrRect* rect);

#endif
