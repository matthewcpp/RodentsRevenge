#ifndef RR_UI_GAME_OVER_H
#define RR_UI_GAME_OVER_H

#include "basic_rect.h"
#include "basic_sprite.h"

typedef struct {
    rrRenderer* _renderer;

    rrUiBasicRect background;
    rrUiBasicSprite text;
    rrUiBasicSprite caption;
} rrUiGameOver;

void rr_ui_game_over_init(rrUiGameOver* game_over, rrRenderer* renderer);
void rr_ui_game_over_draw(rrUiGameOver* game_over);

#endif
