#include "game_ui.h"

#include "../assets.h"

void rr_game_ui_on_pause_dialog_resume(void* user_data);

        rrGameUi* rr_game_ui_create(rrGame* game, rrRenderer* renderer, rrInput* input, rrSpritesheet* spritesheet) {
    rrGameUi* ui = malloc(sizeof(rrGameUi));
    rrPoint ui_element_offset;

    ui->game = game;
    ui->renderer = renderer;
    ui->input = input;

    ui_element_offset.y =  10;
    ui_element_offset.x = -10;
    rr_ui_score_init(&ui->score, &ui_element_offset, game, renderer);

    ui_element_offset.x = 0;
    rr_ui_clock_init(&ui->clock, renderer, game, spritesheet->sprites[RR_SPRITESHEET_INDEX_CLOCK], &ui_element_offset);

    ui_element_offset.x = 10;
    rr_ui_lives_init(&ui->lives, spritesheet->sprites[RR_SPRITESHEET_INDEX_REMAINING_LIFE], &ui_element_offset, game, renderer);

    ui->pause_dialog = rr_ui_pause_dialog_create(renderer, input);
    rr_ui_button_set_callback(&ui->pause_dialog->resume_button, rr_game_ui_on_pause_dialog_resume, ui);

    return ui;
}

void rr_game_ui_destroy(rrGameUi* game_ui) {
    rr_ui_pause_dialog_destroy(game_ui->pause_dialog);
    free(game_ui);
}

void rr_game_ui_show(rrGameUi* game_ui) {
    game_ui->pause_dialog->active = 0;
}

void rr_game_ui_draw(rrGameUi* ui) {
    rr_ui_score_draw(&ui->score);
    rr_ui_clock_draw(&ui->clock);
    rr_ui_lives_draw(&ui->lives);

    if (ui->pause_dialog->active) {
        rr_ui_pause_dialog_draw(ui->pause_dialog);
    }
}

void rr_game_ui_on_pause_dialog_resume(void* user_data) {
    rrGameUi* ui = (rrGameUi*)user_data;

    rr_game_set_paused(ui->game, 0);
    ui->pause_dialog->active = 0;
}

void rr_game_ui_update(rrGameUi* ui) {
    rrGameState game_state = rr_game_get_state(ui->game);

    rr_ui_score_update(&ui->score);

    if (ui->pause_dialog->active) {
        rr_ui_pause_dialog_update(ui->pause_dialog);
    }

    if (game_state == RR_GAME_STATE_PLAYING && rr_input_button_down(ui->input, RR_INPUT_BUTTON_START)) {
        rr_game_set_paused(ui->game, 1);
        rr_ui_pause_dialog_show(ui->pause_dialog);
    }


}
