#ifndef RR_UI_H
#define RR_UI_H

#include "../game.h"
#include "../draw/renderer.h"
#include "../draw/spritesheet.h"

#include "menu.h"
#include "score.h"
#include "clock.h"
#include "lives.h"

typedef struct {
    rrGame* game;
    rrRenderer* renderer;
    rrInput* input;

    rrUiMenu menu;
    rrUiScore score;
    rrUiClock clock;
    rrUILives lives;
} rrUi;

rrUi* rr_ui_create(rrGame* game, rrRenderer* renderer, rrInput* input, rrSpritesheet* spritesheet);
void rr_ui_destroy(rrUi* ui);
void rr_ui_update(rrUi* ui);
void rr_ui_draw(rrUi* ui);

#endif
