#include "menu.h"

#include <stdlib.h>

#define RR_MENU_ITEM_SPACING_H 10
#define RR_MENU_ITEM_SPACING_V 2
#define RR_SUBMENU_PADDING 2

typedef struct {
    rrSprite* sprite;
    cutil_vector* submenuItems;
} rrUiMenuItem;

static rrColor fill_color = {0, 0, 200, 255};

void rr_ui_menu_init(rrUiMenu* menu, rrRenderer* renderer, rrInput* input, void* user_data) {
    menu->items = cutil_vector_create(cutil_trait_ptr());
    menu->selected = 0;
    menu->active_submenu = NULL;
    menu->active_submenu_index = 0;
    menu->user_data = user_data;
    menu->active = 0;
    menu->bar_height = 0;
    menu->_renderer = renderer;
    menu->_input = input;
}

void rr_ui_menu_uninit(rrUiMenu* menu) {
    (void)menu;
}

void rr_ui_menu_add(rrUiMenu* menu, rrSprite* sprite, cutil_vector* submenu_items) {
    rrUiMenuItem* menu_item = malloc(sizeof(rrUiMenuItem));
    menu_item->sprite = sprite;
    menu_item->submenuItems = submenu_items;

    cutil_vector_push_back(menu->items, &menu_item);

    if (sprite->rect.h > menu->bar_height) {
        menu->bar_height = sprite->rect.h;
    }
}


void rr_ui_menu_update_submenu(rrUiMenu* menu) {
    if (rr_input_button_down(menu->_input, RR_INPUT_BUTTON_BACK)) {
        menu->active_submenu = NULL;
        menu->active_submenu_index = 0;
    }

    if (rr_input_button_down(menu->_input, RR_INPUT_BUTTON_ACCEPT)) {
        rrUiSubmenuItem* item;
        cutil_vector_get(menu->active_submenu, menu->active_submenu_index, &item);
        item->callback(menu->user_data);

        menu->active_submenu = NULL;
        menu->active_submenu_index = 0;
    }

    if (rr_input_button_down(menu->_input, RR_INPUT_BUTTON_UP)) {
        menu->active_submenu_index -= menu->active_submenu_index > 0 ? 1 : 0;
    }

    if (rr_input_button_down(menu->_input, RR_INPUT_BUTTON_DOWN)) {
        menu->active_submenu_index += menu->active_submenu_index < cutil_vector_size(menu->active_submenu) - 1 ? 1 : 0;
    }
}

void rr_ui_menu_update_bar(rrUiMenu* menu) {
    rrUiMenuItem* menu_item;
    if (rr_input_button_down(menu->_input, RR_INPUT_BUTTON_RIGHT)) {
        menu->selected += menu->selected < cutil_vector_size(menu->items) - 1 ? 1 : 0;
    }

    if (rr_input_button_down(menu->_input, RR_INPUT_BUTTON_LEFT)) {
        menu->selected -= menu->selected > 0 ? 1 : 0;
    }

    if (rr_input_button_down(menu->_input, RR_INPUT_BUTTON_ACCEPT)) {
        cutil_vector_get(menu->items, menu->selected, &menu_item);
        menu->active_submenu = menu_item->submenuItems;
    }
}

void rr_ui_menu_update(rrUiMenu* menu) {
    if (menu->active_submenu)
        rr_ui_menu_update_submenu(menu);
    else
        rr_ui_menu_update_bar(menu);
}

void rr_ui_menu_draw_menu_bar(rrUiMenu* menu) {
    rrPoint screen_size;
    rrRect menu_bar_rect = {0, 0, 0, 0};
    rrPoint pos = {RR_MENU_ITEM_SPACING_H / 2, RR_MENU_ITEM_SPACING_V};
    rrColor white = {255, 255, 255, 255};
    size_t i;

    menu->active_submenu_pos.y = menu->bar_height;

    rr_renderer_get_screen_size(menu->_renderer, &screen_size);
    menu_bar_rect.w = screen_size.x;
    menu_bar_rect.h = menu->bar_height;

    rr_renderer_color(menu->_renderer, &white);
    rr_renderer_fill_rect(menu->_renderer, &menu_bar_rect);

    for (i = 0; i < cutil_vector_size(menu->items); i++) {
        rrColor color;
        rrUiMenuItem* menu_item;
        cutil_vector_get(menu->items, i, &menu_item);

        if (menu->active && i == menu->selected) {
            rrRect fill_rect;
            fill_rect.x = pos.x;
            fill_rect.y = 0;
            fill_rect.w = menu_item->sprite->rect.w;
            fill_rect.h = menu_bar_rect.h;

            rr_renderer_color(menu->_renderer, &fill_color);
            rr_renderer_fill_rect(menu->_renderer, &fill_rect);

            menu->active_submenu_pos.x = fill_rect.x;

            rr_color_white(&color);
        }
        else {
            rr_color_black(&color);
        }

        rr_renderer_set_sprite_tint_color(menu->_renderer, menu_item->sprite, &color);
        rr_renderer_draw_sprite(menu->_renderer, menu_item->sprite, &pos);

        pos.x += menu_item->sprite->rect.w + RR_MENU_ITEM_SPACING_H;
    }
}

void rr_ui_menu_calculate_submenu_rect(rrUiMenu* menu, rrRect* submenu_rect) {
    int i;
    int count = (int)cutil_vector_size(menu->active_submenu);

    submenu_rect->w = 0;
    for (i = 0; i < count; i++) {
        rrUiSubmenuItem* item;
        cutil_vector_get(menu->active_submenu, i, &item);

        if (item->sprite->rect.w > submenu_rect->w)
            submenu_rect->w = item->sprite->rect.w;
    }
    submenu_rect->w += RR_SUBMENU_PADDING * 2;

    submenu_rect->x = menu->active_submenu_pos.x;
    submenu_rect->y = menu->active_submenu_pos.y;
    submenu_rect->h = menu->bar_height * count + (count * RR_SUBMENU_PADDING * 2);
}

void rr_ui_menu_draw_submenu(rrUiMenu* menu) {
    rrRect submenu_rect;
    rrPoint draw_pos;
    int i;
    int count = (int)cutil_vector_size(menu->active_submenu);
    rrColor color;

    rr_ui_menu_calculate_submenu_rect(menu, &submenu_rect);
    rr_color_white(&color);
    rr_renderer_color(menu->_renderer, &color);
    rr_renderer_fill_rect(menu->_renderer, &submenu_rect);

    draw_pos.x = submenu_rect.x + RR_SUBMENU_PADDING;
    draw_pos.y = submenu_rect.y + RR_SUBMENU_PADDING;

    for (i = 0; i < count; i++) {
        rrUiSubmenuItem *item;
        cutil_vector_get(menu->active_submenu, i, &item);

        if (menu->active_submenu_index == i) {
            rrRect fill_rect;
            fill_rect.x = submenu_rect.x;
            fill_rect.y = draw_pos.y - RR_SUBMENU_PADDING;
            fill_rect.w = submenu_rect.w;
            fill_rect.h = item->sprite->rect.h + (2* RR_SUBMENU_PADDING);

            rr_renderer_color(menu->_renderer, &fill_color);
            rr_renderer_fill_rect(menu->_renderer, &fill_rect);

            rr_color_white(&color);
            rr_renderer_set_sprite_tint_color(menu->_renderer, item->sprite, &color);
        }
        else {
            rr_color_black(&color);
            rr_renderer_set_sprite_tint_color(menu->_renderer, item->sprite, &color);
        }

        rr_renderer_draw_sprite(menu->_renderer, item->sprite, &draw_pos);
        draw_pos.y += menu->bar_height + RR_SUBMENU_PADDING;
    }
}

void rr_ui_menu_draw(rrUiMenu* menu) {
    rr_ui_menu_draw_menu_bar(menu);

    if (menu->active_submenu) {
        rr_ui_menu_draw_submenu(menu);
    }
}

rrUiSubmenuItem* rr_ui_submenu_item_create(rrSprite* sprite, rrUiMenuCallbackFunc callback) {
    rrUiSubmenuItem* item = malloc(sizeof(rrUiSubmenuItem));

    item->sprite = sprite;
    item->callback = callback;

    return item;
}
