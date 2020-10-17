#include "game_ui.h"

#include "../assets.h"

void rr_game_ui_layout(rrGameUi* ui, rrSpritesheet* spritesheet);
void rr_game_ui_on_pause_dialog_resume(void* user_data);
void rr_game_ui_on_high_score_accepted(void* user_data);

rrGameUi* rr_game_ui_create(rrGame* game, rrHighScores* high_scores, rrRenderer* renderer, rrInput* input, rrSpritesheet* spritesheet) {
    rrGameUi* ui = malloc(sizeof(rrGameUi));

    ui->game = game;
    ui->high_scores = high_scores;
    ui->renderer = renderer;
    ui->input = input;

    ui->_previous_state = rr_game_get_state(game);

    rr_game_ui_layout(ui, spritesheet);

    return ui;
}

void rr_game_ui_destroy(rrGameUi* game_ui) {
    rr_ui_pause_dialog_destroy(game_ui->pause_dialog);
    rr_ui_high_score_dialog_delete(game_ui->high_score_dialog);
    free(game_ui);
}

void rr_game_ui_layout(rrGameUi* ui, rrSpritesheet* spritesheet) {
    rrPoint ui_element_offset;

    ui_element_offset.y =  10;
    ui_element_offset.x = -10;
    rr_ui_score_init(&ui->score, &ui_element_offset, ui->game, ui->renderer);

    ui_element_offset.x = 0;
    rr_ui_clock_init(&ui->clock, ui->renderer, ui->game, rr_renderer_get_sprite(ui->renderer, RR_SPRITE_CLOCK), &ui_element_offset);

    ui_element_offset.x = 10;
    rr_ui_lives_init(&ui->lives, spritesheet->sprites[RR_SPRITESHEET_INDEX_REMAINING_LIFE], &ui_element_offset, ui->game, ui->renderer);

    ui->pause_dialog = rr_ui_pause_dialog_create(ui->renderer, ui->input);
    rr_ui_button_set_callback(&ui->pause_dialog->resume_button, rr_game_ui_on_pause_dialog_resume, ui);

    ui->high_score_dialog = rr_ui_high_score_dialog_create(ui->renderer, ui->input);
    rr_ui_button_set_callback(&ui->high_score_dialog->ok_button, rr_game_ui_on_high_score_accepted, ui);
}

void rr_game_ui_show(rrGameUi* game_ui) {
    game_ui->pause_dialog->active = 0;
    game_ui->high_score_dialog->active = 0;
}

void rr_game_ui_draw(rrGameUi* ui) {
    rr_ui_score_draw(&ui->score);
    rr_ui_clock_draw(&ui->clock);
    rr_ui_lives_draw(&ui->lives);

    if (ui->pause_dialog->active)
        rr_ui_pause_dialog_draw(ui->pause_dialog);

    if (ui->high_score_dialog->active)
        rr_ui_high_score_dialog_draw(ui->high_score_dialog);
}

void rr_game_ui_on_pause_dialog_resume(void* user_data) {
    rrGameUi* ui = (rrGameUi*)user_data;

    rr_game_set_paused(ui->game, 0);
    ui->pause_dialog->active = 0;
}

void rr_game_ui_update(rrGameUi* ui) {
    rrGameState game_state = rr_game_get_state(ui->game);

    if (ui->pause_dialog->active) {
        rr_ui_pause_dialog_update(ui->pause_dialog);
    }
    else if (ui->high_score_dialog->active) {
        rr_ui_high_score_dialog_update(ui->high_score_dialog);
    }
    else {
        rr_ui_score_update(&ui->score);

        if (game_state == RR_GAME_STATE_PLAYING && rr_input_button_down(ui->input, RR_INPUT_BUTTON_START)) {
            rr_game_set_paused(ui->game, 1);
            rr_ui_pause_dialog_show(ui->pause_dialog);
        }
    }

    if (game_state == RR_GAME_STATE_OVER && game_state != ui->_previous_state) {
        rrPlayer* player = rr_game_get_player(ui->game);

        if (rr_high_scores_score_is_high(ui->high_scores, player->score)) {
            rr_ui_high_score_dialog_show(ui->high_score_dialog);
        }
    }

    ui->_previous_state = game_state;
}

void rr_game_ui_on_high_score_accepted(void* user_data) {
    rrGameUi* ui = (rrGameUi*)user_data;
    rrPlayer* player = rr_game_get_player(ui->game);
    const char* player_name = rr_ui_text_get_str(&ui->high_score_dialog->text_input);
    ui->high_score_dialog->active = 0;

    rr_high_scores_add(ui->high_scores, player->score, player_name);
    rr_high_scores_write(ui->high_scores);
}
