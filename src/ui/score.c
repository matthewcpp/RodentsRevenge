#include "score.h"

#include <stdio.h>

void rr_ui_score_init(rrUiScore* score, rrPoint* offset, rrGame* game, rrRenderer* renderer) {
    score->sprite = NULL;
    score->prev_score = -1;
    score->_game = game;
    score->_renderer = renderer;

    rr_point_copy(&score->offset, offset);
}

void rr_ui_score_uninit(rrUiScore* score) {
    if (score->sprite)
        rr_renderer_destroy_sprite(score->_renderer, score->sprite);
}

void rr_ui_score_update(rrUiScore* score) {
    rrPlayer* player = rr_game_get_player(score->_game);

    if (player->score != score->prev_score) {
        rrColor color;
        rrPoint screen_size;
        char buffer[8];
        sprintf(buffer, "%d", player->score);

        if (score->sprite)
            rr_renderer_destroy_sprite(score->_renderer, score->sprite);

        score->sprite = rr_renderer_create_text(score->_renderer, 0, buffer);
        rr_color_black(&color);
        rr_renderer_set_sprite_tint_color(score->_renderer, score->sprite, &color);

        rr_renderer_get_screen_size(score->_renderer, &screen_size);
        rr_point_set(&score->pos, screen_size.x - score->sprite->rect.w, 0);
        rr_point_add(&score->pos, &score->pos, &score->offset);
    }
}

void rr_ui_score_draw(rrUiScore* score) {
    rr_renderer_draw_sprite(score->_renderer, score->sprite, &score->pos);
}
