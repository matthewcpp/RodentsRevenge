#include "basic_sprite.h"

void rr_ui_basic_sprite_init(rrUiBasicSprite* basic_sprite, rrPoint* position, rrSprite* sprite, rrRenderer* renderer) {
    rr_point_copy(&basic_sprite->element.position, position);
    basic_sprite->sprite = sprite;
    basic_sprite->_renderer = renderer;
}
void rr_ui_basic_sprite_draw(rrUiBasicSprite* basic_sprite) {
    rr_renderer_draw_sprite(basic_sprite->_renderer, basic_sprite->sprite, &basic_sprite->element.position);
}

void rr_ui_basic_sprite_get_rect(rrUiBasicSprite* basic_sprite, rrRect* rect) {
    rect->x = basic_sprite->element.position.x;
    rect->y = basic_sprite->element.position.y;
    rect->w = basic_sprite->sprite->rect.w;
    rect->h = basic_sprite->sprite->rect.h;
}
