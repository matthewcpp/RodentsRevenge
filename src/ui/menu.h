#ifndef RR_UI_MENU_H
#define RR_UI_MENU_H

#include "../draw/renderer.h"
#include "../input.h"

#include "cutil/vector.h"

typedef enum {
    RR_MENU_GAME,
    RR_MENU_OPTIONS,
    RR_MENU_COUNT
} rrUiMenuIndex;

typedef void (*rrUiMenuCallbackFunc)(void* data);

typedef struct {
    rrSprite* sprite;
    rrUiMenuCallbackFunc callback;
} rrUiSubmenuItem;

rrUiSubmenuItem* rr_ui_submenu_item_create(rrSprite* sprite, rrUiMenuCallbackFunc callback);

typedef struct {
    int selected;

    rrSprite* items[RR_MENU_COUNT];
    cutil_vector* submenus[RR_MENU_COUNT];

    rrPoint active_submenu_pos;
    cutil_vector* active_submenu;
    int active_submenu_index;

    void* user_data;
    int bar_height;
} rrUiMenu;

void rr_ui_menu_init(rrUiMenu* menu, void* user_data);
void rr_ui_menu_uninit(rrUiMenu* menu);
void rr_ui_menu_draw(rrUiMenu* menu, rrRenderer* renderer);
void rr_ui_menu_update(rrUiMenu* menu, rrInput* input);

#endif
