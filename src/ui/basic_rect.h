#ifndef RR_UI_BASIC_RECT_H
#define RR_UI_BASIC_RECT_H

#include "../draw/renderer.h"

typedef struct {
    rrRect rect;
    rrColor color;
    rrRenderer* _renderer;
} rrUiBasicRect;

void rr_ui_basic_rect_init(rrUiBasicRect* basic_rect, rrRect* rect, rrColor* color, rrRenderer* renderer);
void rr_ui_basic_rect_draw(rrUiBasicRect* basic_rect);

#endif
