#ifndef RR_HIGH_SCORE_DIALOG_H
#define RR_HIGH_SCORE_DIALOG_H

#include "button.h"
#include "text_input.h"
#include "basic_rect.h"
#include "basic_sprite.h"

#include "../high_scores.h"

typedef struct {
    rrInput* _input;
    rrRenderer* _renderer;
    rrUiBasicSprite prompt_line_1;
    rrUiBasicSprite prompt_line_2;
    rrUiTextInput text_input;
    rrUiButton ok_button;
    int active;
    rrUiActiveElementGroup _element_group;
    rrRect _layout_rect;
    rrUiBasicRect bar;
} rrUiHighScoreDialog;

rrUiHighScoreDialog* rr_ui_high_score_dialog_create(rrRenderer* renderer, rrInput* input);
void rr_ui_high_score_dialog_delete(rrUiHighScoreDialog* high_score_dialog);
void rr_ui_high_score_dialog_show(rrUiHighScoreDialog* high_score_dialog);
void rr_ui_high_score_dialog_update(rrUiHighScoreDialog* high_score_dialog);
void rr_ui_high_score_dialog_draw(rrUiHighScoreDialog* high_score_dialog);

#endif
