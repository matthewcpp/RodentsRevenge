#include "title_ui.h"

void rr_title_ui_layout(rrTitleUi* ui, rrSprite* app_icon, rrRenderer* renderer);

rrTitleUi* rr_title_ui_create(rrSprite* app_icon, rrRenderer* renderer, rrGame* game, rrInput* input) {
    rrTitleUi* ui = malloc(sizeof(rrTitleUi));

    ui->_game = game;
    ui->_renderer = renderer;
    ui->_input = input;

    rr_title_ui_layout(ui, app_icon, renderer);

    ui->active_index = 0;
    ui->buttons[ui->active_index].active = 1;

    return ui;
}

void rr_title_ui_set_active_index(rrTitleUi* ui, int index) {
    ui->buttons[ui->active_index].active = 0;
    ui->buttons[index].active = 1;
    ui->active_index = index;
}

void rr_title_ui_update(rrTitleUi* ui) {
    if (rr_input_button_down(ui->_input, RR_INPUT_BUTTON_DOWN) && ui->active_index < RR_TITLE_UI_BUTTON_COUNT - 1)
        rr_title_ui_set_active_index(ui, ui->active_index + 1);

    if (rr_input_button_down(ui->_input, RR_INPUT_BUTTON_UP) && ui->active_index > 0 )
        rr_title_ui_set_active_index(ui, ui->active_index - 1);

    if (rr_input_button_down(ui->_input, RR_INPUT_BUTTON_ACCEPT))
        rr_ui_button_activate(&ui->buttons[ui->active_index]);
}

void rr_title_ui_layout(rrTitleUi* ui, rrSprite* app_icon, rrRenderer* renderer) {
    rrSprite* app_text_sprite;
    rrUiButton* button;
    rrColor color;
    rrPoint screen_size, render_pos, button_size;
    rr_renderer_get_screen_size(ui->_renderer, & screen_size);

    render_pos.x = screen_size.x / 2 - app_icon->rect.w / 2;
    render_pos.y = 10;
    rr_ui_basic_sprite_init(&ui->app_icon, &render_pos, app_icon, renderer);

    app_text_sprite = rr_renderer_create_text(renderer, 0, "Rodent's Revenge");
    rr_color_black(&color);
    rr_renderer_set_sprite_tint_color(ui->_renderer, app_text_sprite, &color);
    render_pos.y += ui->app_icon.sprite->rect.h + 10;
    render_pos.x = screen_size.x / 2 - app_text_sprite->rect.w / 2;
    rr_ui_basic_sprite_init(&ui->app_text, &render_pos, app_text_sprite, renderer);

    button = &ui->buttons[0];
    render_pos.y += ui->app_text.sprite->rect.h + 15;
    rr_ui_button_init(button, renderer, "New Game", &render_pos);
    rr_ui_button_get_size(button, &button_size);
    button->pos.x = screen_size.x / 2 - button_size.x / 2;

    button = &ui->buttons[1];
    render_pos.y += button_size.y + 5;
    rr_ui_button_init(button, renderer, "Pick Level", &render_pos);
    rr_ui_button_get_size(button, &button_size);
    button->pos.x = screen_size.x / 2 - button_size.x / 2;
}

void rr_title_ui_draw(rrTitleUi* ui) {
    int i;
    rr_ui_basic_sprite_draw(&ui->app_icon);
    rr_ui_basic_sprite_draw(&ui->app_text);

    for (i = 0; i < 2; i++)
        rr_ui_button_draw(&ui->buttons[i]);
}
