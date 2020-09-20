#include "game_ui.h"

#include "../assets.h"

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

    return ui;
}

void rr_game_ui_draw(rrGameUi* ui) {
    rr_ui_score_draw(&ui->score);
    rr_ui_clock_draw(&ui->clock);
    rr_ui_lives_draw(&ui->lives);
}

void rr_game_ui_update(rrGameUi* ui) {
    rr_ui_score_update(&ui->score);
}
