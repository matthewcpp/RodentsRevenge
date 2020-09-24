#include "title_ui.h"
#include "../assets.h"

#include <assert.h>

void rr_title_ui_layout(rrTitleUi* ui);

rrTitleUi* rr_title_ui_create(rrRenderer* renderer, rrGame* game, rrInput* input) {

    rrTitleUi* ui = malloc(sizeof(rrTitleUi));

    ui->_game = game;
    ui->_renderer = renderer;
    ui->_input = input;

    rr_title_ui_layout(ui);

    ui->active_index = 0;
    ui->buttons[ui->active_index].element.active = 1;

    return ui;
}

void rr_title_ui_set_active_index(rrTitleUi* ui, int index) {
    ui->buttons[ui->active_index].element.active = 0;
    ui->buttons[index].element.active = 1;
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

void rr_title_ui_layout(rrTitleUi* ui) {
    rrSprite* app_text_sprite;
    rrSprite* app_icon;
    rrUiButton* button;
    rrColor color;
    rrPoint screen_size, render_pos, button_size;
    rr_renderer_get_screen_size(ui->_renderer, & screen_size);

    app_icon = rr_renderer_get_sprite(ui->_renderer, RR_SPRITE_TITLE_LOGO);
    assert(app_icon != NULL);

    render_pos.x = screen_size.x / 2 - app_icon->rect.w / 2;
    render_pos.y = 10;
    rr_ui_basic_sprite_init(&ui->app_icon, &render_pos, app_icon, ui->_renderer);

    app_text_sprite = rr_renderer_create_text(ui->_renderer, RR_FONT_TITLE, "Rodent's Revenge");
    rr_color_black(&color);
    rr_renderer_set_sprite_tint_color(ui->_renderer, app_text_sprite, &color);
    render_pos.y += ui->app_icon.sprite->rect.h + 10;
    render_pos.x = screen_size.x / 2 - app_text_sprite->rect.w / 2;
    rr_ui_basic_sprite_init(&ui->app_text, &render_pos, app_text_sprite, ui->_renderer);

    button = &ui->buttons[0];
    render_pos.y += ui->app_text.sprite->rect.h + 15;
    rr_ui_button_init(button, ui->_renderer, "New Game", &render_pos);
    rr_ui_button_get_size(button, &button_size);
    button->element.position.x = screen_size.x / 2 - button_size.x / 2;

    button = &ui->buttons[1];
    render_pos.y += button_size.y + 5;
    rr_ui_button_init(button, ui->_renderer, "Pick Level...", &render_pos);
    rr_ui_button_get_size(button, &button_size);
    button->element.position.x = screen_size.x / 2 - button_size.x / 2;
}

void rr_title_ui_draw(rrTitleUi* ui) {
    int i;
    rr_ui_basic_sprite_draw(&ui->app_icon);
    rr_ui_basic_sprite_draw(&ui->app_text);

    for (i = 0; i < 2; i++)
        rr_ui_button_draw(&ui->buttons[i]);
}
