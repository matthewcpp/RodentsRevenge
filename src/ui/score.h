#ifndef RR_UI_SCORE_H
#define RR_UI_SCORE_H

#include "../game.h"
#include "../draw/renderer.h"

typedef struct {
    rrPoint pos;
    rrPoint offset;
    rrSprite * sprite;
    int prev_score;
    rrGame* _game;
    rrRenderer* _renderer;
}rrUiScore;

void rr_ui_score_init(rrUiScore* score, rrPoint* top_right_offset, rrGame* game, rrRenderer* renderer);
void rr_ui_score_update(rrUiScore* score);
void rr_ui_score_draw(rrUiScore* score);

#endif
