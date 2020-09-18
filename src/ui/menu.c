#include "menu.h"

#define RR_MENU_ITEM_SPACING_H 10
#define RR_MENU_ITEM_SPACING_V 2

void rr_ui_menu_init(rrUiMenu* menu, rrRenderer* renderer) {
    menu->items[RR_MENU_GAME] = rr_renderer_create_text(renderer, 0, "Game");
    menu->items[RR_MENU_OPTIONS] = rr_renderer_create_text(renderer, 0, "Options");

    menu->bar_height = menu->items[RR_MENU_GAME]->rect.h + RR_MENU_ITEM_SPACING_V * 2;

    menu->selected = 0;
}

void rr_ui_menu_update(rrUiMenu* menu, rrInput* input) {
    if (rr_input_button_down(input, RR_INPUT_BUTTON_RIGHT)) {
        menu->selected += menu->selected < RR_MENU_COUNT - 1 ? 1 : 0;
    }

    if (rr_input_button_down(input, RR_INPUT_BUTTON_LEFT)) {
        menu->selected -= menu->selected > 0 ? 1 : 0;
    }
}

void rr_ui_menu_draw(rrUiMenu* menu, rrRenderer* renderer) {
    rrPoint screen_size;
    rrRect menu_bar_rect = {0, 0, 0, 0};
    rrPoint pos = {RR_MENU_ITEM_SPACING_H / 2, RR_MENU_ITEM_SPACING_V};
    rrColor white = {255, 255, 255, 255};
    int i;

    rr_renderer_get_screen_size(renderer, &screen_size);
    menu_bar_rect.w = screen_size.x;
    menu_bar_rect.h = menu->bar_height;

    rr_renderer_color(renderer, &white);
    rr_renderer_fill_rect(renderer, &menu_bar_rect);

    for (i = 0; i < RR_MENU_COUNT; i++) {
        rrColor color;
        rrSprite* menu_sprite = menu->items[i];

        if (i == menu->selected) {
            rrColor fill_color = {0, 0, 200, 255};
            rrRect fill_rect;
            fill_rect.x = pos.x;
            fill_rect.y = 0;
            fill_rect.w = menu_sprite->rect.w;
            fill_rect.h = menu_bar_rect.h;

            rr_renderer_color(renderer, &fill_color);
            rr_renderer_fill_rect(renderer, &fill_rect);

            rr_color_white(&color);
        }
        else {
            rr_color_black(&color);
        }

        rr_renderer_set_sprite_tint_color(renderer, menu_sprite, &color);
        rr_renderer_draw_sprite(renderer, menu_sprite, &pos);

        pos.x += menu_sprite->rect.w + RR_MENU_ITEM_SPACING_H;
    }
}
