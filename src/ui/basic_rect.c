#include "basic_rect.h"

void rr_ui_basic_rect_init(rrUiBasicRect* basic_rect, rrRect* rect, rrColor* color, rrRenderer* renderer){
    basic_rect->rect = *rect;
    basic_rect->color = *color;
    basic_rect->_renderer = renderer;
}

void rr_ui_basic_rect_draw(rrUiBasicRect* basic_rect) {
    rr_renderer_color(basic_rect->_renderer, &basic_rect->color);
    rr_renderer_fill_rect(basic_rect->_renderer, &basic_rect->rect);
}
