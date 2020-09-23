#include "button.h"
#include "../assets.h"

#include <stddef.h>

rrPoint button_size = {225, 70};

void rr_ui_button_init(rrUiButton* button, rrRenderer* renderer, const char* text, rrPoint* pos){
    rrColor color;
    rr_color_black(&color);

    button->_renderer = renderer;
    rr_point_copy(&button->element.position, pos);

    button->sprite = rr_renderer_create_text(renderer, RR_FONT_BUTTON, text);
    rr_renderer_set_sprite_tint_color(renderer, button->sprite, &color);

    button->element.active = 0;
    button->_callback = NULL;
    button->_user_data = NULL;
}

void rr_ui_button_draw(rrUiButton* button) {
    rrRect button_rect, inset_rect;
    rrPoint text_point;
    rrColor color;

    button_rect.x = button->element.position.x;
    button_rect.y = button->element.position.y;
    button_rect.w = button->sprite->rect.w + 50;
    button_rect.h = button->sprite->rect.h + 30;

    rr_color_set(&color, 130, 130, 130, 255);
    rr_renderer_color(button->_renderer, &color);
    rr_renderer_fill_rect(button->_renderer, &button_rect);

    inset_rect = button_rect;
    inset_rect.w -= 3;
    inset_rect.h -= 3;

    rr_color_white(&color);
    rr_renderer_color(button->_renderer, &color);
    rr_renderer_fill_rect(button->_renderer, &inset_rect);

    rr_color_set(&color, 195, 195, 195, 255);
    rr_renderer_color(button->_renderer, &color);

    inset_rect = button_rect;
    inset_rect.x += 3;
    inset_rect.y += 3;
    inset_rect.w -= 6;
    inset_rect.h -= 6;
    rr_renderer_fill_rect(button->_renderer, &inset_rect);

    rr_color_black(&color);
    rr_renderer_color(button->_renderer, &color);
    rr_renderer_draw_rect(button->_renderer, &button_rect);

    text_point.x = button_rect.x + 25;
    text_point.y = button_rect.y + 15;
    rr_renderer_draw_sprite(button->_renderer, button->sprite, &text_point);

    if (button->element.active) {
        rrRect active_rect;
        active_rect.x = text_point.x;
        active_rect.y = text_point.y;
        active_rect.w = button->sprite->rect.w;
        active_rect.h = button->sprite->rect.h;

        rr_renderer_draw_rect(button->_renderer, &active_rect);
    }
}

void rr_ui_button_get_size(rrUiButton* button, rrPoint* size) {
    size->x = button->sprite->rect.w + 50;
    size->y = button->sprite->rect.h + 30;
}

void rr_ui_button_set_callback(rrUiButton* button, rrUiButtonCallbackFunc callback, void* user_data) {
    button->_callback = callback;
    button->_user_data = user_data;
}

void rr_ui_button_activate(rrUiButton* button) {
    if (button->_callback) {
        button->_callback(button->_user_data);
    }
}
