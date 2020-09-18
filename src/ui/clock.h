#ifndef RR_UI_CLOCK_H
#define RR_UI_CLOCK_H

#include "../draw/renderer.h"
#include "../game.h"

typedef struct {
    rrRenderer* _renderer;
    rrGame* _game;
    rrSprite* _sprite;
    rrPoint offset;
    rrPoint pos;
} rrUiClock;

void rr_ui_clock_init(rrUiClock* clock, rrRenderer* renderer, rrGame* game, rrSprite* sprite, rrPoint* offset);
void rr_ui_clock_draw(rrUiClock* clock);

#endif
