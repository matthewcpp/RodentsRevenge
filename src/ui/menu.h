#ifndef RR_UI_MENU_H
#define RR_UI_MENU_H

#include "../draw/renderer.h"
#include "../input.h"

typedef enum {
    RR_MENU_GAME,
    RR_MENU_OPTIONS,
    RR_MENU_COUNT
} rrUiMenuIndex;

typedef struct {
    int selected;

    rrSprite* items[RR_MENU_COUNT];
    int bar_height;
} rrUiMenu;

void rr_ui_menu_init(rrUiMenu* menu, rrRenderer* renderer);
void rr_ui_menu_draw(rrUiMenu* menu, rrRenderer* renderer);
void rr_ui_menu_update(rrUiMenu* menu, rrInput* input);

#endif
