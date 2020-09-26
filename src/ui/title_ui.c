#include "title_ui.h"
#include "../assets.h"

#include <assert.h>

void rr_title_ui_layout(rrTitleUi* ui);
void rr_title_ui_on_level_select_button(void* user_data);
void rr_title_ui_on_level_select_cancel(void* user_data);

rrTitleUi* rr_title_ui_create(rrRenderer* renderer, rrGame* game, rrInput* input) {

    rrTitleUi* ui = malloc(sizeof(rrTitleUi));

    ui->_game = game;
    ui->_renderer = renderer;
    ui->_input = input;

    rr_ui_active_element_group_init(&ui->_element_group);

    rr_title_ui_layout(ui);
    ui->level_select_dialog = rr_ui_level_select_create(game, renderer, input);
    rr_ui_button_set_callback(&ui->level_select_dialog->cancel_button, rr_title_ui_on_level_select_cancel, ui);

    return ui;
}

void rr_title_ui_destroy(rrTitleUi* ui) {
    rr_ui_active_element_group_uninit(&ui->_element_group);
    rr_ui_level_select_delete(ui->level_select_dialog);
    free(ui);
}

void rr_title_ui_show(rrTitleUi* ui) {
    rr_ui_active_element_group_set(&ui->_element_group, 0);
    ui->level_select_dialog->active = 0;
}

void rr_title_ui_update(rrTitleUi* ui) {
    if (ui->level_select_dialog->active) {
        rr_ui_level_select_update(ui->level_select_dialog);
        return;
    }

    if (rr_input_button_down(ui->_input, RR_INPUT_BUTTON_DOWN))
        rr_ui_active_element_group_next(&ui->_element_group);

    if (rr_input_button_down(ui->_input, RR_INPUT_BUTTON_UP))
        rr_ui_active_element_group_previous(&ui->_element_group);

    if (rr_input_button_down(ui->_input, RR_INPUT_BUTTON_ACCEPT)){
        rrUiButton* button = (rrUiButton*)rr_ui_active_element_group_get_current(&ui->_element_group);
        rr_ui_button_activate(button);
    }
}

void rr_title_ui_layout(rrTitleUi* ui) {
    rrSprite* app_text_sprite;
    rrSprite* app_icon;
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

    render_pos.y += ui->app_text.sprite->rect.h + 15;
    rr_ui_button_init(&ui->new_game_button, ui->_renderer, "New Game", &render_pos);
    rr_ui_active_element_group_add(&ui->_element_group, &ui->new_game_button.element);
    rr_ui_button_get_size(&ui->new_game_button, &button_size);
    ui->new_game_button.element.position.x = screen_size.x / 2 - button_size.x / 2;

    render_pos.y += button_size.y + 5;
    rr_ui_button_init(&ui->level_select_button, ui->_renderer, "Pick Level...", &render_pos);
    rr_ui_button_set_callback(&ui->level_select_button, rr_title_ui_on_level_select_button, ui);
    rr_ui_active_element_group_add(&ui->_element_group, &ui->level_select_button.element);
    rr_ui_button_get_size(&ui->level_select_button, &button_size);
    ui->level_select_button.element.position.x = screen_size.x / 2 - button_size.x / 2;
}

void rr_title_ui_draw(rrTitleUi* ui) {
    rr_ui_basic_sprite_draw(&ui->app_icon);
    rr_ui_basic_sprite_draw(&ui->app_text);

    rr_ui_button_draw(&ui->new_game_button);
    rr_ui_button_draw(&ui->level_select_button);

    if (ui->level_select_dialog->active) {
        rr_ui_level_select_draw(ui->level_select_dialog);
    }
}

void rr_title_ui_on_level_select_button(void* user_data) {
    rrTitleUi* ui = (rrTitleUi*)user_data;

    rr_ui_level_select_show(ui->level_select_dialog);
    ui->level_select_dialog->active = 1;
}

void rr_title_ui_on_level_select_cancel(void* user_data) {
    rrTitleUi* ui = (rrTitleUi*)user_data;
    ui->level_select_dialog->active = 0;
}
