#include "level_select_ui.h"
#include "../assets.h"

#include <stdio.h>

void rr_ui_level_select_layout(rrUILevelSelect* level_select);

rrUILevelSelect* rr_ui_level_select_create(rrGame* game, rrRenderer* renderer, rrInput* input) {
    rrUILevelSelect* level_select = malloc(sizeof(rrUILevelSelect));

    level_select->_input = input;
    level_select->_game = game;
    level_select->_renderer = renderer;
    level_select->current_level = 1;

    rr_ui_active_element_group_init(&level_select->element_group);
    rr_ui_level_select_layout(level_select);

    return level_select;
}

void rr_ui_level_select_delete(rrUILevelSelect* level_select) {
    rr_ui_active_alement_group_uninit(&level_select->element_group);
}

void rr_ui_level_select_update_active_level_text(rrUILevelSelect* level_select) {
    if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_DOWN))
        rr_ui_active_alement_group_next(&level_select->element_group);
}

void rr_ui_level_select_update_active_buttons(rrUILevelSelect* level_select) {
    if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_RIGHT))
        rr_ui_active_alement_group_next(&level_select->element_group);
    else if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_LEFT))
        rr_ui_active_alement_group_previous(&level_select->element_group);
    else if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_UP))
        rr_ui_active_alement_group_set(&level_select->element_group, 0);
}


void rr_ui_level_select_update(rrUILevelSelect* level_select) {
    if (level_select->level_text.element.active)
        rr_ui_level_select_update_active_level_text(level_select);
    else
        rr_ui_level_select_update_active_buttons(level_select);
}

void rr_ui_level_select_layout(rrUILevelSelect* level_select) {
    rrSprite* text_sprite;
    rrPoint position = {10, 10};
    rrPoint button_size;
    char text_buffer[64];
    rrColor color;
    rr_color_black(&color);

    sprintf(text_buffer, "Select a level (1-%d): ", rr_game_get_level_count(level_select->_game));
    text_sprite = rr_renderer_create_text(level_select->_renderer, RR_FONT_BUTTON, text_buffer);
    rr_renderer_set_sprite_tint_color(level_select->_renderer, text_sprite, &color);
    rr_ui_basic_sprite_init(&level_select->level_prompt, &position, text_sprite, level_select->_renderer);
    position.y += text_sprite->rect.h + 10;

    sprintf(text_buffer, "%d ", level_select->current_level);
    text_sprite = rr_renderer_create_text(level_select->_renderer, RR_FONT_BUTTON, text_buffer);
    level_select->level_text.element.active = 1;
    rr_renderer_set_sprite_tint_color(level_select->_renderer, text_sprite, &color);
    rr_ui_basic_sprite_init(&level_select->level_text, &position, text_sprite, level_select->_renderer);
    rr_ui_active_alement_group_add(&level_select->element_group, &level_select->level_text.element);
    position.y += text_sprite->rect.h + 10;

    rr_ui_button_init(&level_select->ok_button, level_select->_renderer, "Ok", &position);
    rr_ui_button_get_size(&level_select->ok_button, &button_size);
    rr_ui_active_alement_group_add(&level_select->element_group, &level_select->ok_button.element);
    position.x += button_size.x + 10;

    rr_ui_button_init(&level_select->cancel_button, level_select->_renderer, "Cancel", &position);
    rr_ui_active_alement_group_add(&level_select->element_group, &level_select->cancel_button.element);
}

void rr_level_select_draw_level_text(rrUILevelSelect* level_select) {
    rrColor color;
    rrRect text_rect;
    rr_ui_basic_sprite_get_rect(&level_select->level_text, &text_rect);

    if (level_select->level_text.element.active) {
        rr_color_blue(&color);
        rr_renderer_color(level_select->_renderer, &color);
        rr_renderer_fill_rect(level_select->_renderer, &text_rect);

        rr_color_white(&color);
    }
    else {
        rr_color_white(&color);
        rr_renderer_color(level_select->_renderer, &color);
        rr_renderer_fill_rect(level_select->_renderer, &text_rect);

        rr_color_black(&color);
    }

    rr_renderer_set_sprite_tint_color(level_select->_renderer, level_select->level_text.sprite, &color);
    rr_ui_basic_sprite_draw(&level_select->level_text);
}

void rr_ui_level_select_draw(rrUILevelSelect* level_select) {
    rr_ui_basic_sprite_draw(&level_select->level_prompt);
    rr_level_select_draw_level_text(level_select);

    rr_ui_button_draw(&level_select->ok_button);
    rr_ui_button_draw(&level_select->cancel_button);
}
