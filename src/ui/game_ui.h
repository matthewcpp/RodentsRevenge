#ifndef RR_UI_H
#define RR_UI_H

#include "../game.h"
#include "../draw/renderer.h"
#include "../draw/spritesheet.h"

#include "score.h"
#include "clock.h"
#include "lives.h"
#include "pause_dialog.h"

typedef struct {
    rrGame* game;
    rrRenderer* renderer;
    rrInput* input;

    rrUiScore score;
    rrUiClock clock;
    rrUILives lives;

    rrUIPauseDialog* pause_dialog;
} rrGameUi;

rrGameUi* rr_game_ui_create(rrGame* game, rrRenderer* renderer, rrInput* input, rrSpritesheet* spritesheet);
void rr_game_ui_destroy(rrGameUi* game_ui);
void rr_game_ui_show(rrGameUi* game_ui);
void rr_game_ui_update(rrGameUi* ui);
void rr_game_ui_draw(rrGameUi* ui);

#endif
