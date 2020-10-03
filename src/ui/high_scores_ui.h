#ifndef RR_HIGH_SCORES_UI_H
#define RR_HIGH_SCORES_UI_H

#include "../input.h"
#include "../high_scores.h"

#include "button.h"
#include "basic_sprite.h"
#include "element.h"

typedef struct {
    rrRenderer* _renderer;
    rrInput* _input;
    rrHighScores* _scores;

    rrUiButton back_button;
    rrUiButton clear_button;
    rrUiBasicSprite title;
    rrUiBasicSprite trophies[3];
    rrUiActiveElementGroup _element_group;
} rrHighScoresUi;

rrHighScoresUi* rr_high_scores_ui_create(rrRenderer* renderer, rrHighScores* scores, rrInput* input);
void rr_high_scores_ui_destroy(rrHighScoresUi* high_scores_ui);

void rr_high_scores_ui_show(rrHighScoresUi* high_scores_ui);
void rr_high_scores_ui_update(rrHighScoresUi* high_scores_ui);
void rr_high_scores_ui_draw(rrHighScoresUi* high_scores_ui);

#endif
