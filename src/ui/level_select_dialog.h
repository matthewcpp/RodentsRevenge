#ifndef RR_UI_LEVEL_SELECT_H
#define RR_UI_LEVEL_SELECT_H

#include "../game.h"
#include "../input.h"
#include "../draw/renderer.h"
#include "basic_sprite.h"
#include "basic_rect.h"
#include "button.h"

typedef struct {
    rrInput* _input;
    rrRenderer* _renderer;
    rrGame* _game;

    int current_level;
    int active;

    rrRect layout_rect;

    rrUiBasicRect bar;
    rrUiBasicSprite level_prompt;
    rrUiBasicSprite level_text;
    rrUiButton ok_button;
    rrUiButton cancel_button;
    rrUiActiveElementGroup element_group;
} rrUILevelSelect;

rrUILevelSelect* rr_ui_level_select_create(rrGame* game, rrRenderer* renderer, rrInput* input);
void rr_ui_level_select_delete(rrUILevelSelect* level_select);
void rr_ui_level_select_show(rrUILevelSelect* level_select);
void rr_ui_level_select_update(rrUILevelSelect* level_select);
void rr_ui_level_select_draw(rrUILevelSelect* level_select);

#endif
