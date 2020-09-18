#include "menu.h"

#include <stdlib.h>

#define RR_MENU_ITEM_SPACING_H 10
#define RR_MENU_ITEM_SPACING_V 2
#define RR_SUBMENU_PADDING 2

void rr_ui_menu_on_new_game(void* menu);
void rr_ui_menu_on_select_level(void* menu);

void rr_ui_menu_init_game_menu(rrUiMenu* menu, rrRenderer* renderer);
void rr_ui_menu_init_options_menu(rrUiMenu* menu, rrRenderer* renderer);

static rrColor fill_color = {0, 0, 200, 255};

void rr_ui_menu_init(rrUiMenu* menu, rrRenderer* renderer) {
    rr_ui_menu_init_game_menu(menu, renderer);
    rr_ui_menu_init_options_menu(menu, renderer);

    menu->bar_height = menu->items[RR_MENU_GAME]->rect.h + RR_MENU_ITEM_SPACING_V * 2;
    menu->active_submenu_pos.y = menu->bar_height;

    menu->selected = 0;
    menu->active_submenu = NULL;
    menu->active_submenu_index = 0;
}

void rr_ui_menu_uninit(rrUiMenu* menu) {
    (void)menu;
}

void rr_ui_menu_init_game_menu(rrUiMenu* menu, rrRenderer* renderer) {
    rrUiSubmenuItem* submenu_item;
    cutil_vector* submenu = cutil_vector_create(cutil_trait_ptr());

    menu->items[RR_MENU_GAME] = rr_renderer_create_text(renderer, 0, "Game");
    menu->submenus[RR_MENU_GAME] = submenu;

    submenu_item = rr_ui_submenu_item_create(rr_renderer_create_text(renderer, 0, "New Game"), rr_ui_menu_on_new_game);
    cutil_vector_push_back(submenu, &submenu_item);
    submenu_item = rr_ui_submenu_item_create(rr_renderer_create_text(renderer, 0, "High Scores..."), rr_ui_menu_on_new_game);
    cutil_vector_push_back(submenu, &submenu_item);
}

void rr_ui_menu_init_options_menu(rrUiMenu* menu, rrRenderer* renderer) {
    rrUiSubmenuItem* submenu_item;
    cutil_vector* submenu = cutil_vector_create(cutil_trait_ptr());

    menu->items[RR_MENU_OPTIONS] = rr_renderer_create_text(renderer, 0, "Options");
    menu->submenus[RR_MENU_OPTIONS] = submenu;

    submenu_item = rr_ui_submenu_item_create(rr_renderer_create_text(renderer, 0, "Level..."), rr_ui_menu_on_select_level);
    cutil_vector_push_back(submenu, &submenu_item);
}

void rr_ui_menu_update_submenu(rrUiMenu* menu, rrInput* input) {
    if (rr_input_button_down(input, RR_INPUT_BUTTON_BACK)) {
        menu->active_submenu = NULL;
        menu->active_submenu_index = 0;
    }

    if (rr_input_button_down(input, RR_INPUT_BUTTON_UP)) {
        menu->active_submenu_index -= menu->active_submenu_index > 0 ? 1 : 0;
    }

    if (rr_input_button_down(input, RR_INPUT_BUTTON_DOWN)) {
        menu->active_submenu_index += menu->active_submenu_index < cutil_vector_size(menu->active_submenu) - 1 ? 1 : 0;
    }
}

void rr_ui_menu_update_bar(rrUiMenu* menu, rrInput* input) {
    if (rr_input_button_down(input, RR_INPUT_BUTTON_RIGHT)) {
        menu->selected += menu->selected < RR_MENU_COUNT - 1 ? 1 : 0;
    }

    if (rr_input_button_down(input, RR_INPUT_BUTTON_LEFT)) {
        menu->selected -= menu->selected > 0 ? 1 : 0;
    }

    if (rr_input_button_down(input, RR_INPUT_BUTTON_ACCEPT)) {
        menu->active_submenu = menu->submenus[menu->selected];
    }
}

void rr_ui_menu_update(rrUiMenu* menu, rrInput* input) {
    if (menu->active_submenu)
        rr_ui_menu_update_submenu(menu, input);
    else
        rr_ui_menu_update_bar(menu, input);
}

void rr_ui_menu_draw_menu_bar(rrUiMenu* menu, rrRenderer* renderer) {
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
            rrRect fill_rect;
            fill_rect.x = pos.x;
            fill_rect.y = 0;
            fill_rect.w = menu_sprite->rect.w;
            fill_rect.h = menu_bar_rect.h;

            rr_renderer_color(renderer, &fill_color);
            rr_renderer_fill_rect(renderer, &fill_rect);

            menu->active_submenu_pos.x = fill_rect.x;

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

void rr_ui_menu_draw_submenu(rrUiMenu* menu, rrRenderer* renderer) {
    rrRect submenu_rect;
    rrPoint draw_pos;
    int i;
    int count = (int)cutil_vector_size(menu->active_submenu);
    rrColor color;

    rr_ui_menu_calculate_submenu_rect(menu, &submenu_rect);
    rr_color_white(&color);
    rr_renderer_color(renderer, &color);
    rr_renderer_fill_rect(renderer, &submenu_rect);

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

            rr_renderer_color(renderer, &fill_color);
            rr_renderer_fill_rect(renderer, &fill_rect);

            rr_color_white(&color);
            rr_renderer_set_sprite_tint_color(renderer, item->sprite, &color);
        }
        else {
            rr_color_black(&color);
            rr_renderer_set_sprite_tint_color(renderer, item->sprite, &color);
        }

        rr_renderer_draw_sprite(renderer, item->sprite, &draw_pos);
        draw_pos.y += menu->bar_height + RR_SUBMENU_PADDING;
    }
}

void rr_ui_menu_draw(rrUiMenu* menu, rrRenderer* renderer) {
    rr_ui_menu_draw_menu_bar(menu, renderer);

    if (menu->active_submenu) {
        rr_ui_menu_draw_submenu(menu, renderer);
    }
}

rrUiSubmenuItem* rr_ui_submenu_item_create(rrSprite* sprite, rrUiMenuCallbackFunc callback) {
    rrUiSubmenuItem* item = malloc(sizeof(rrUiSubmenuItem));

    item->sprite = sprite;
    item->callback = callback;

    return item;
}

void rr_ui_menu_on_new_game(void* menu){
    (void)menu;
}

void rr_ui_menu_on_select_level(void* menu) {
    (void)menu;
}

