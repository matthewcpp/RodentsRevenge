#include "text_input.h"
#include "../assets.h"

#define TEXT_INPUT_PADDING 2

void on_keyboard_key(int key, void* user_data);
void on_keyboard_bakspace(void* user_data);
void on_keyboard_done(void* user_data);

void rr_ui_text_input_init_keyboard_sprite(rrUiTextInput* text_input);

void rr_ui_text_input_init(rrUiTextInput* text_input, rrInput* input,  rrRenderer* renderer, rrPoint* pos) {
    text_input->_input = input;
    text_input->_renderer = renderer;
    text_input->strbuf = cutil_strbuf_create_with_cstring("W");
    text_input->width_override = 0;

    rr_point_copy(&text_input->element.position, pos);
    text_input->element.active = 0;
    text_input->onscreen_keyboard = rr_ui_onscreen_keyboard_create(renderer, input);
    text_input->onscreen_keyboard->on_char = on_keyboard_key;
    text_input->onscreen_keyboard->on_backspace = on_keyboard_bakspace;
    text_input->onscreen_keyboard->on_done = on_keyboard_done;
    text_input->onscreen_keyboard->user_data = text_input;

    // 'W' used for layout measurement purposes.
    rr_ui_text_input_init_keyboard_sprite(text_input);
    cutil_strbuf_pop_back(text_input->strbuf);
}

void rr_ui_text_input_uninit(rrUiTextInput* text_input) {
    cutil_strbuf_destroy(text_input->strbuf);
    rr_ui_onscreen_keyboard_destroy(text_input->onscreen_keyboard);
}

void rr_ui_text_input_draw(rrUiTextInput* text_input) {
    rrColor color;
    rrRect draw_rect;
    rrPoint draw_pos;
    rr_ui_text_input_get_rect(text_input, &draw_rect);

    rr_color_white(&color);
    rr_renderer_color(text_input->_renderer, &color);
    rr_renderer_fill_rect(text_input->_renderer, &draw_rect);

    rr_point_copy(&draw_pos, &text_input->element.position);
    draw_pos.x += TEXT_INPUT_PADDING;
    draw_pos.y += TEXT_INPUT_PADDING;

    if (cutil_strbuf_length(text_input->strbuf) > 0)
        rr_renderer_draw_sprite(text_input->_renderer, text_input->_text_sprite, &draw_pos);

    if (text_input->onscreen_keyboard->active) {
        rr_ui_onscreen_keyboard_draw(text_input->onscreen_keyboard);
    }
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

void rr_ui_text_input_init_keyboard_sprite(rrUiTextInput* text_input) {
    rrColor color;
    rr_color_black(&color);

    text_input->_text_sprite = rr_renderer_create_text(text_input->_renderer, RR_FONT_BUTTON, cutil_strbuf_cstring(text_input->strbuf));
    rr_renderer_set_sprite_tint_color(text_input->_renderer, text_input->_text_sprite, &color);
}

void rr_ui_text_input_update(rrUiTextInput* text_input) {
    if (!text_input->element.active)
        return;

    if (rr_input_button_down(text_input->_input, RR_INPUT_BUTTON_ACCEPT) && !text_input->onscreen_keyboard->active)
        rr_ui_onscreen_keyboard_show(text_input->onscreen_keyboard);
    else if (text_input->onscreen_keyboard->active) {
        rr_ui_onscreen_keyboard_update(text_input->onscreen_keyboard);
    }
}

void on_keyboard_key(int key, void* user_data) {
    rrUiTextInput* text_input = (rrUiTextInput*)user_data;

    cutil_strbuf_append_char(text_input->strbuf, (char)key);
    rr_renderer_update_text_sprite(text_input->_renderer, text_input->_text_sprite, RR_FONT_BUTTON, cutil_strbuf_cstring(text_input->strbuf));
}

void on_keyboard_bakspace(void* user_data) {
    rrUiTextInput* text_input = (rrUiTextInput*)user_data;
    cutil_strbuf_pop_back(text_input->strbuf);
    rr_renderer_update_text_sprite(text_input->_renderer, text_input->_text_sprite, RR_FONT_BUTTON, cutil_strbuf_cstring(text_input->strbuf));
}

void on_keyboard_done(void* user_data) {
    rrUiTextInput* text_input = (rrUiTextInput*)user_data;
    text_input->onscreen_keyboard->active = 0;
}
