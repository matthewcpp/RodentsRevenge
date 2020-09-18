#include "ui.h"

void rr_ui_menu_init_game_menu(rrUiMenu* menu, rrRenderer* renderer);
void rr_ui_menu_init_options_menu(rrUiMenu* menu, rrRenderer* renderer);

rrUi* rr_ui_create(rrGame* game, rrRenderer* renderer, rrInput* input) {
    rrUi* ui = malloc(sizeof(rrUi));

    ui->game = game;
    ui->renderer = renderer;
    ui->input = input;

    rr_ui_menu_init(&ui->menu, ui);
    rr_ui_menu_init_game_menu(&ui->menu, renderer);
    rr_ui_menu_init_options_menu(&ui->menu, renderer);

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

void rr_ui_menu_on_new_game(void* user_data) {
    rrUi* ui = (rrUi*)user_data;

    rr_game_reset(ui->game);
    rr_game_restart(ui->game);
}

void rr_ui_menu_on_select_level(void* user_data) {
    (void)user_data;
}

void rr_ui_menu_init_game_menu(rrUiMenu* menu, rrRenderer* renderer) {
    rrUiSubmenuItem* submenu_item;
    cutil_vector* submenu = cutil_vector_create(cutil_trait_ptr());

    menu->items[RR_MENU_GAME] = rr_renderer_create_text(renderer, 0, "Game");
    menu->submenus[RR_MENU_GAME] = submenu;

    submenu_item = rr_ui_submenu_item_create(rr_renderer_create_text(renderer, 0, "New Game"), rr_ui_menu_on_new_game);
    cutil_vector_push_back(submenu, &submenu_item);
    submenu_item = rr_ui_submenu_item_create(rr_renderer_create_text(renderer, 0, "High Scores..."), rr_ui_menu_on_new_game);
    cutil_vector_push_back(submenu, &submenu_item);
}

void rr_ui_menu_init_options_menu(rrUiMenu* menu, rrRenderer* renderer) {
    rrUiSubmenuItem* submenu_item;
    cutil_vector* submenu = cutil_vector_create(cutil_trait_ptr());

    menu->items[RR_MENU_OPTIONS] = rr_renderer_create_text(renderer, 0, "Options");
    menu->submenus[RR_MENU_OPTIONS] = submenu;

    submenu_item = rr_ui_submenu_item_create(rr_renderer_create_text(renderer, 0, "Level..."), rr_ui_menu_on_select_level);
    cutil_vector_push_back(submenu, &submenu_item);
}
