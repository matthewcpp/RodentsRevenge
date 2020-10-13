#include "score.h"
#include "../assets.h"

#include <stdio.h>

void rr_ui_score_update_sprite(rrUiScore* score);

void rr_ui_score_init(rrUiScore* score, rrPoint* offset, rrGame* game, rrRenderer* renderer) {
    score->sprite = NULL;
    score->prev_score = -1;
    score->_player = rr_game_get_player(game);
    score->_renderer = renderer;

    rr_point_copy(&score->offset, offset);
    rr_ui_score_update_sprite(score);
}

void rr_ui_score_layout(rrUiScore* score) {
    rrPoint screen_size;

    rr_renderer_get_screen_size(score->_renderer, &screen_size);
    rr_point_set(&score->pos, screen_size.x - score->sprite->rect.w, 0);
    rr_point_add(&score->pos, &score->pos, &score->offset);
}

void rr_ui_score_update_sprite(rrUiScore* score) {
    rrColor color;
    char buffer[8];
    sprintf(buffer, "%d", score->_player->score);

    if (score->sprite == NULL){
        score->sprite = rr_renderer_create_text(score->_renderer, RR_FONT_SCORE, buffer);
        rr_color_black(&color);
        rr_renderer_set_sprite_tint_color(score->_renderer, score->sprite, &color);
    }
    else {
        rr_renderer_update_text_sprite(score->_renderer, score->sprite, RR_FONT_SCORE, buffer);
    }

    rr_ui_score_layout(score);
}

void rr_ui_score_update(rrUiScore* score) {
    if (score->_player->score != score->prev_score) {
        rr_ui_score_update_sprite(score);
        score->prev_score = score->_player->score;
    }
}

void rr_ui_score_draw(rrUiScore* score) {
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &score->pos);

    rr_renderer_draw_sprite(score->_renderer, score->sprite, &draw_pos);
    draw_pos.x += 1;
    rr_renderer_draw_sprite(score->_renderer, score->sprite, &draw_pos);
}
