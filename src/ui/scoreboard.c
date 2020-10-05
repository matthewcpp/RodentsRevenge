#include "scoreboard.h"

#include "../assets.h"

#include <stdio.h>

void rr_ui_scoreboard_init(rrUiScoreboard* scoreboard, rrHighScores* high_scores, rrRenderer* renderer) {
    scoreboard->_high_scores = high_scores;
    scoreboard->_renderer = renderer;
}

void rr_ui_scoreboard_layout(rrUiScoreboard* scoreboard, rrPoint* position) {
    size_t i;
    rrPoint element_pos;
    rrColor text_color;

    rr_color_black(&text_color);
    rr_point_copy(&element_pos, position);
    element_pos.x = 150;

    for(i = 0; i< SCOREBOARD_SIZE; i++) {
        rrSprite* name = rr_renderer_create_text(scoreboard->_renderer, RR_FONT_BUTTON, "Player");
        rrSprite* score = rr_renderer_create_text(scoreboard->_renderer, RR_FONT_BUTTON, "Score");

        rr_renderer_set_sprite_tint_color(scoreboard->_renderer, name, &text_color);
        rr_renderer_set_sprite_tint_color(scoreboard->_renderer, score, &text_color);

        rr_ui_basic_sprite_init(&scoreboard->names[i], &element_pos, name, scoreboard->_renderer);
        rr_ui_basic_sprite_init(&scoreboard->scores[i], &element_pos, score, scoreboard->_renderer);

        element_pos.y += 31;
    }

    rr_ui_scoreboard_update(scoreboard);
}

void rr_ui_scoreboard_update(rrUiScoreboard* scoreboard) {
    size_t i, count = rr_high_scores_count(scoreboard->_high_scores);
    rrPoint screen_size;
    char score_str[16];

    rr_renderer_get_screen_size(scoreboard->_renderer, &screen_size);

    for (i = 0; i < count; i++) {
        rrUiBasicSprite* name_sprite = &scoreboard->names[i];
        rrUiBasicSprite* score_sprite = &scoreboard->scores[i];
        rrPoint pos;
        char* name;
        int score;

        rr_high_scores_get_score(scoreboard->_high_scores, i, &name, &score);
        rr_renderer_update_text_sprite(scoreboard->_renderer, name_sprite->sprite, RR_FONT_BUTTON, name);
        sprintf(score_str, "%d", score);
        rr_renderer_update_text_sprite(scoreboard->_renderer, score_sprite->sprite, RR_FONT_BUTTON, score_str);

        pos.y = score_sprite->element.position.y;
        pos.x = screen_size.x - score_sprite->sprite->rect.w - 150;
        rr_point_copy(&score_sprite->element.position, &pos);
    }
}

void rr_ui_scoreboard_draw(rrUiScoreboard* scoreboard) {
    size_t i, count = rr_high_scores_count(scoreboard->_high_scores);

    for (i = 0; i < count; i++) {
        rr_ui_basic_sprite_draw(&scoreboard->names[i]);
        rr_ui_basic_sprite_draw(&scoreboard->scores[i]);
    }
}
