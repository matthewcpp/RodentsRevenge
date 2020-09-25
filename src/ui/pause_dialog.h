#ifndef RR_UI_PAUSE_DIALOG_H
#define RR_UI_PAUSE_DIALOG_H

#include "../game.h"
#include "../input.h"
#include "../draw/renderer.h"
#include "basic_sprite.h"
#include "basic_rect.h"
#include "button.h"

typedef struct {
    rrInput* _input;
    rrRenderer* _renderer;

    rrRect layout_rect;
    int active;

    rrUiBasicRect bar;
    rrUiBasicSprite level_prompt;
    rrUiButton resume_button;
    rrUiButton exit_button;
    rrUiActiveElementGroup element_group;
} rrUIPauseDialog;

rrUIPauseDialog* rr_ui_pause_dialog_create(rrRenderer* renderer, rrInput* input);
void rr_ui_pause_dialog_show(rrUIPauseDialog* pause_dialog);
void rr_ui_pause_dialog_destroy(rrUIPauseDialog* pause_dialog);
void rr_ui_pause_dialog_update(rrUIPauseDialog* pause_dialog);
void rr_ui_pause_dialog_draw(rrUIPauseDialog* pause_dialog);

#endif
