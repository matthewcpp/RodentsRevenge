#include "common.h"

void rr_ui_draw_inset_rect(rrRenderer* renderer, rrRect* rect) {
    rrColor color;
    rrRect inset_rect;

    rr_color_set(&color, 130, 130, 130, 255);
    rr_renderer_color(renderer, &color);
    rr_renderer_fill_rect(renderer, rect);

    inset_rect = *rect;
    inset_rect.w -= 3;
    inset_rect.h -= 3;

    rr_color_white(&color);
    rr_renderer_color(renderer, &color);
    rr_renderer_fill_rect(renderer, &inset_rect);

    rr_color_set(&color, 195, 195, 195, 255);
    rr_renderer_color(renderer, &color);

    inset_rect = *rect;
    inset_rect.x += 3;
    inset_rect.y += 3;
    inset_rect.w -= 6;
    inset_rect.h -= 6;
    rr_renderer_fill_rect(renderer, &inset_rect);
}
