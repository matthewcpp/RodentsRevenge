#include "ui.h"

void rr_ui_menu_init_game_menu(rrUiMenu* menu, rrRenderer* renderer);
void rr_ui_menu_init_options_menu(rrUiMenu* menu, rrRenderer* renderer);

rrUi* rr_ui_create(rrGame* game, rrRenderer* renderer, rrInput* input) {
    rrUi* ui = malloc(sizeof(rrUi));
    rrPoint ui_element_offset;

    ui->game = game;
    ui->renderer = renderer;
    ui->input = input;

    rr_ui_menu_init(&ui->menu, renderer, input, ui);
    rr_ui_menu_init_game_menu(&ui->menu, renderer);
    rr_ui_menu_init_options_menu(&ui->menu, renderer);

    ui_element_offset.y = ui->menu.bar_height + 10;
    ui_element_offset.x = -10;

    rr_ui_score_init(&ui->score, &ui_element_offset, game, renderer);
    ui_element_offset.x = 0;
    rr_ui_clock_init(&ui->clock, renderer, game, NULL, &ui_element_offset);

    return ui;
}

void rr_ui_destroy(rrUi* ui) {
    rr_ui_menu_uninit(&ui->menu);
    rr_ui_score_uninit(&ui->score);
}

void rr_ui_draw(rrUi* ui) {
    ui->menu.active = rr_game_get_state(ui->game) == RR_GAME_STATE_PAUSED;
    rr_ui_menu_draw(&ui->menu);
    rr_ui_score_draw(&ui->score);
}

void rr_ui_update(rrUi* ui) {
    rr_ui_menu_update(&ui->menu);
    rr_ui_score_update(&ui->score);
}

void rr_ui_menu_on_new_game(void* user_data) {
    rrUi* ui = (rrUi*)user_data;

    rr_game_reset(ui->game);
    rr_game_restart(ui->game);
}

void rr_ui_menu_on_select_level(void* user_data) {
    (void)user_data;
}

void rr_ui_menu_init_game_menu(rrUiMenu* menu, rrRenderer* renderer) {
    rrSprite* sprite = rr_renderer_create_text(renderer, 0, "Game");
    cutil_vector* submenu = cutil_vector_create(cutil_trait_ptr());
    rrUiSubmenuItem* submenu_item;

    submenu_item = rr_ui_submenu_item_create(rr_renderer_create_text(renderer, 0, "New Game"), rr_ui_menu_on_new_game);
    cutil_vector_push_back(submenu, &submenu_item);
    submenu_item = rr_ui_submenu_item_create(rr_renderer_create_text(renderer, 0, "High Scores..."), rr_ui_menu_on_new_game);
    cutil_vector_push_back(submenu, &submenu_item);

    rr_ui_menu_add(menu, sprite, submenu);
}

void rr_ui_menu_init_options_menu(rrUiMenu* menu, rrRenderer* renderer) {
    rrSprite* sprite = rr_renderer_create_text(renderer, 0, "Options");
    cutil_vector* submenu = cutil_vector_create(cutil_trait_ptr());
    rrUiSubmenuItem* submenu_item;

    submenu_item = rr_ui_submenu_item_create(rr_renderer_create_text(renderer, 0, "Level..."), rr_ui_menu_on_select_level);
    cutil_vector_push_back(submenu, &submenu_item);

    rr_ui_menu_add(menu, sprite, submenu);
}
