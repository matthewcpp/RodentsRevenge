#ifndef RR_TITLE_UI_H
#define RR_TITLE_UI_H

#include "button.h"
#include "basic_sprite.h"
#include "element.h"
#include "level_select_dialog.h"

#include "../game.h"
#include "../input.h"
#include "../draw/renderer.h"


typedef struct {
    rrUiBasicSprite app_icon;
    rrUiBasicSprite app_text;
    rrUiButton new_game_button;
    rrUiButton level_select_button;
    rrUiButton high_scores_button;
    rrUiActiveElementGroup _element_group;
    rrUILevelSelect* level_select_dialog;

    rrRenderer * _renderer;
    rrGame* _game;
    rrInput* _input;
} rrTitleUi;


rrTitleUi* rr_title_ui_create(rrRenderer* renderer, rrGame* game, rrInput* input);
void rr_title_ui_destroy(rrTitleUi* ui);
void rr_title_ui_show(rrTitleUi* ui);

void rr_title_ui_update(rrTitleUi* ui);
void rr_title_ui_draw(rrTitleUi* ui);

#endif
