#ifndef RR_BASIC_SPRITE_H
#define RR_BASIC_SPRITE_H

#include "../draw/renderer.h"

typedef struct {
    rrPoint position;
    rrSprite* sprite;
    rrRenderer* _renderer;
} rrUiBasicSprite;

void rr_ui_basic_sprite_init(rrUiBasicSprite* basic_sprite, rrPoint* position, rrSprite* sprite, rrRenderer* renderer);
void rr_ui_basic_sprite_draw(rrUiBasicSprite* basic_sprite);

#endif
