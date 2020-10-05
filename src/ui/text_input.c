#include "text_input.h"
#include "../assets.h"

#define TEXT_INPUT_PADDING 2

void rr_ui_text_input_init(rrUiTextInput* text_input, rrInput* input,  rrRenderer* renderer, rrPoint* pos, const char* str) {
    rrColor color;
    text_input->_input = input;
    text_input->_renderer = renderer;
    text_input->strbuf = cutil_strbuf_create_with_cstring(str);
    text_input->width_override = 0;

    rr_point_copy(&text_input->element.position, pos);
    text_input->element.active = 0;

    rr_color_black(&color);
    text_input->_text_sprite = rr_renderer_create_text(renderer, RR_FONT_BUTTON, str);
    rr_renderer_set_sprite_tint_color(renderer, text_input->_text_sprite, &color);
}

void rr_ui_text_input_uninit(rrUiTextInput* text_input) {
    cutil_strbuf_destroy(text_input->strbuf);
}

void rr_ui_text_input_buffer_updated(rrUiTextInput* textInput) {
    rr_renderer_update_text_sprite(textInput->_renderer, textInput->_text_sprite, RR_FONT_BUTTON, cutil_strbuf_cstring(textInput->strbuf));
}

void rr_ui_text_input_draw(rrUiTextInput* text_input) {
    rrColor color;
    rrRect draw_rect;
    rr_ui_text_input_get_rect(text_input, &draw_rect);

    rr_color_white(&color);
    rr_renderer_color(text_input->_renderer, &color);
    rr_renderer_fill_rect(text_input->_renderer, &draw_rect);

    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &text_input->element.position);
    draw_pos.x += TEXT_INPUT_PADDING;
    draw_pos.y += TEXT_INPUT_PADDING;

    rr_renderer_draw_sprite(text_input->_renderer, text_input->_text_sprite, &draw_pos);
}

void rr_ui_text_input_get_rect(rrUiTextInput* text_input, rrRect* rect) {
    rect->x = text_input->element.position.x;
    rect->y = text_input->element.position.y;

    if (text_input->width_override > 0)
        rect->w = text_input->width_override + (2 * TEXT_INPUT_PADDING);
    else
        rect->w = text_input->_text_sprite->rect.w + (2 * TEXT_INPUT_PADDING);

    rect->h = text_input->_text_sprite->rect.h + (2 * TEXT_INPUT_PADDING);
}
