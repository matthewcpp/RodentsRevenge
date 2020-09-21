#ifndef RR_TITLE_UI_H
#define RR_TITLE_UI_H

#include "button.h"
#include "basic_sprite.h"

#include "../game.h"
#include "../input.h"
#include "../draw/renderer.h"

#define RR_TITLE_UI_BUTTON_COUNT 2

typedef struct {
    rrUiBasicSprite app_icon;
    rrUiBasicSprite app_text;
    rrUiButton buttons[RR_TITLE_UI_BUTTON_COUNT];

    rrRenderer * _renderer;
    rrGame* _game;
    rrInput* _input;

    int active_index;
} rrTitleUi;

/* TODO: UI should be able to get app icon on its own. */
rrTitleUi* rr_title_ui_create(rrRenderer* renderer, rrGame* game, rrInput* input);

void rr_title_ui_update(rrTitleUi* ui);
void rr_title_ui_draw(rrTitleUi* ui);

#endif
