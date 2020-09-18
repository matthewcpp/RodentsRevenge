#include "ui.h"

rrUi* rr_ui_create(rrGame* game, rrRenderer* renderer, rrInput* input) {
    rrUi* ui = malloc(sizeof(rrUi));

    ui->game = game;
    ui->renderer = renderer;
    ui->input = input;

    rr_ui_menu_init(&ui->menu, renderer);

    return ui;
}

void rr_ui_destroy(rrUi* ui) {
    rr_ui_menu_uninit(&ui->menu);
}

void rr_ui_draw(rrUi* ui) {
    if (rr_game_get_state(ui->game) != RR_GAME_STATE_PAUSED)
        return;

    rr_ui_menu_draw(&ui->menu, ui->renderer);
}

void rr_ui_update(rrUi* ui) {
    if (rr_game_get_state(ui->game) != RR_GAME_STATE_PAUSED)
        return;

    rr_ui_menu_update(&ui->menu, ui->input);
}
