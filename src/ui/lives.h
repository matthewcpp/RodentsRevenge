#ifndef RR_UI_LIVES_H
#define RR_UI_LIVES_H

#include "../game.h"
#include "../draw/renderer.h"

typedef struct {
    rrSprite* sprite;
    rrPoint offset;
    rrGame* _game;
    rrRenderer* _renderer;
} rrUILives;

void rr_ui_lives_init(rrUILives* lives, rrSprite* sprite, rrPoint* top_left_offset, rrGame* game, rrRenderer* renderer);
void rr_ui_lives_draw(rrUILives* lives);
#endif
