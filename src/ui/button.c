#include "button.h"
#include "../assets.h"
#include "common.h"

#include <stddef.h>

rrPoint button_size = {225, 70};

#define RR_UI_BUTTON_PADDING_H 25
#define RR_UI_BUTTON_PADDING_V 15

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
    rrRect button_rect;
    rrPoint text_point;
    rrColor color;

    rr_ui_button_get_rect(button, &button_rect);

    rr_color_set(&color, 130, 130, 130, 255);
    rr_renderer_color(button->_renderer, &color);
    rr_renderer_fill_rect(button->_renderer, &button_rect);

    rr_ui_draw_inset_rect(button->_renderer, &button_rect);

    rr_color_black(&color);
    rr_renderer_color(button->_renderer, &color);
    rr_renderer_draw_rect(button->_renderer, &button_rect);

    text_point.x = button_rect.x + RR_UI_BUTTON_PADDING_H;
    text_point.y = button_rect.y + RR_UI_BUTTON_PADDING_V;
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
    size->x = button->sprite->rect.w + RR_UI_BUTTON_PADDING_H * 2;
    size->y = button->sprite->rect.h + RR_UI_BUTTON_PADDING_V * 2;
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

void rr_ui_button_get_rect(rrUiButton* button, rrRect* rect) {
    rect->x = button->element.position.x;
    rect->y = button->element.position.y;
    rect->w = button->sprite->rect.w + RR_UI_BUTTON_PADDING_H * 2;
    rect->h = button->sprite->rect.h + RR_UI_BUTTON_PADDING_V * 2;
}

int rr_ui_button_hit(rrUiButton* button, rrPoint* point) {
    rrRect rect;
    rr_ui_button_get_rect(button, &rect);

    return rr_rect_contains_point(&rect, point->x, point->y);
}

int rr_ui_button_try_click(rrUiButton* button, rrPoint* point) {
    if (rr_ui_button_hit(button, point)){
        rr_ui_button_activate(button);

        return 1;
    }

    return 0;
}