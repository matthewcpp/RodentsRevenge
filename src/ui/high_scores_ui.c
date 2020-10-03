#include "high_scores_ui.h"

#include "../assets.h"

#include <stdlib.h>

void rr_high_scores_ui_layout(rrHighScoresUi* high_scores_ui);
void rr_high_scores_ui_on_clear_scores(void* user_data);

rrHighScoresUi* rr_high_scores_ui_create(rrRenderer* renderer, rrHighScores* scores, rrInput* input){
    rrHighScoresUi* high_scores_ui = malloc(sizeof(rrHighScoresUi));

    high_scores_ui->_scores = scores;
    high_scores_ui->_renderer = renderer;
    high_scores_ui->_input = input;

    rr_ui_active_element_group_init(&high_scores_ui->_element_group);
    rr_ui_scoreboard_init(&high_scores_ui->scoreboard, scores, renderer);

    rr_high_scores_ui_layout(high_scores_ui);

    return high_scores_ui;
}

void rr_high_scores_ui_destroy(rrHighScoresUi* high_scores_ui){
    rr_ui_active_element_group_uninit(&high_scores_ui->_element_group);
    free(high_scores_ui);
}

void rr_high_scores_ui_layout(rrHighScoresUi* high_scores_ui) {
    rrSprite* trophy_sprite = rr_renderer_get_sprite(high_scores_ui->_renderer, RR_SPRITE_TROPHY);
    rrSprite* text;
    rrColor color;
    rrPoint screen_size, element_size, element_pos;
    int i;

    rr_renderer_get_screen_size(high_scores_ui->_renderer, &screen_size);

    text = rr_renderer_create_text(high_scores_ui->_renderer, RR_FONT_TITLE, "Hall Of Fame");
    rr_color_black(&color);
    rr_renderer_set_sprite_tint_color(high_scores_ui->_renderer, text, &color);
    rr_point_set(&element_pos, screen_size.x / 2 - text->rect.w / 2, 5);
    rr_ui_basic_sprite_init(&high_scores_ui->title, &element_pos, text, high_scores_ui->_renderer);

    element_pos.x = 5;
    element_pos.y += text->rect.h + 10;

    rr_ui_scoreboard_layout(&high_scores_ui->scoreboard, &element_pos);

    element_pos.x = screen_size.x - 5;
    element_pos.y = screen_size.y - 5;

    rr_ui_button_init(&high_scores_ui->clear_button, high_scores_ui->_renderer, "Clear High Scores", &element_pos);
    rr_ui_button_get_size(&high_scores_ui->clear_button, &element_size);
    rr_point_sub(&element_pos, &element_pos, &element_size);
    rr_point_copy(&high_scores_ui->clear_button.element.position, &element_pos);

    rr_ui_button_init(&high_scores_ui->back_button, high_scores_ui->_renderer, "Back", &element_pos);
    rr_ui_button_get_size(&high_scores_ui->back_button, &element_size);
    element_pos.x -= element_size.x + 10;
    rr_point_copy(&high_scores_ui->back_button.element.position, &element_pos);

    rr_point_set(&element_pos, 10, screen_size.y - trophy_sprite->rect.h - 5);
    for (i = 0; i < 3; i++) {
        rr_ui_basic_sprite_init(&high_scores_ui->trophies[i], &element_pos, trophy_sprite, high_scores_ui->_renderer);
        element_pos.x += trophy_sprite->rect.w + 10;
    }

    rr_ui_active_element_group_add(&high_scores_ui->_element_group, &high_scores_ui->back_button.element);
    rr_ui_active_element_group_add(&high_scores_ui->_element_group, &high_scores_ui->clear_button.element);

    rr_ui_button_set_callback(&high_scores_ui->clear_button, rr_high_scores_ui_on_clear_scores, high_scores_ui);
}

void rr_high_scores_ui_update_scoreboard(rrHighScoresUi* high_scores_ui) {

}

void rr_high_scores_ui_show(rrHighScoresUi* high_scores_ui) {
    rr_high_scores_ui_update_scoreboard(high_scores_ui);
}

void rr_high_scores_ui_update_pointer(rrHighScoresUi* high_scores_ui) {
    rrPoint pos;

    if (!rr_input_pointer_up(high_scores_ui->_input))
        return;

    rr_input_pointer_pos(high_scores_ui->_input, &pos);

    rr_ui_button_try_click(&high_scores_ui->clear_button, &pos);
    rr_ui_button_try_click(&high_scores_ui->back_button, &pos);
}

void rr_high_scores_ui_update_input(rrHighScoresUi* high_scores_ui) {
    if (rr_input_button_down(high_scores_ui->_input, RR_INPUT_BUTTON_LEFT))
        rr_ui_active_element_group_previous(&high_scores_ui->_element_group);

    if (rr_input_button_down(high_scores_ui->_input, RR_INPUT_BUTTON_RIGHT))
        rr_ui_active_element_group_next(&high_scores_ui->_element_group);

    if (rr_input_button_down(high_scores_ui->_input, RR_INPUT_BUTTON_ACCEPT)){
        rrUiButton* button = (rrUiButton*)rr_ui_active_element_group_get_current(&high_scores_ui->_element_group);
        rr_ui_button_activate(button);
    }
}

void rr_high_scores_ui_update(rrHighScoresUi* high_scores_ui) {
    rr_high_scores_ui_update_pointer(high_scores_ui);
    rr_high_scores_ui_update_input(high_scores_ui);
}

void rr_high_scores_ui_draw(rrHighScoresUi* high_scores_ui) {
    int i;

    for (i = 0; i < 3; i++) {
        rr_ui_basic_sprite_draw(&high_scores_ui->trophies[i]);
    }

    rr_ui_scoreboard_draw(&high_scores_ui->scoreboard);

    rr_ui_basic_sprite_draw(&high_scores_ui->title);
    rr_ui_button_draw(&high_scores_ui->clear_button);
    rr_ui_button_draw(&high_scores_ui->back_button);
}

void rr_high_scores_ui_on_clear_scores(void* user_data) {
    rrHighScoresUi* high_scores_ui = (rrHighScoresUi*)user_data;

    rr_high_scores_clear(high_scores_ui->_scores);
    rr_high_scores_ui_update_scoreboard(high_scores_ui);
}
