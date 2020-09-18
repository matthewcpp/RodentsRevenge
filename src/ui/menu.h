#ifndef RR_UI_MENU_H
#define RR_UI_MENU_H

#include "../draw/renderer.h"
#include "../input.h"

#include "cutil/vector.h"

typedef void (*rrUiMenuCallbackFunc)(void* data);

typedef struct {
    rrSprite* sprite;
    rrUiMenuCallbackFunc callback;
} rrUiSubmenuItem;

rrUiSubmenuItem* rr_ui_submenu_item_create(rrSprite* sprite, rrUiMenuCallbackFunc callback);

typedef struct {
    int selected;

    cutil_vector* items;

    rrPoint active_submenu_pos;
    cutil_vector* active_submenu;
    int active_submenu_index;

    void* user_data;
    int bar_height;
    int active;

    rrRenderer* _renderer;
    rrInput* _input;
} rrUiMenu;

void rr_ui_menu_init(rrUiMenu* menu, rrRenderer* renderer, rrInput* input, void* user_data);
void rr_ui_menu_uninit(rrUiMenu* menu);
void rr_ui_menu_add(rrUiMenu* menu, rrSprite* sprite, cutil_vector* submenu_items);
void rr_ui_menu_draw(rrUiMenu* menu);
void rr_ui_menu_update(rrUiMenu* menu);

#endif
