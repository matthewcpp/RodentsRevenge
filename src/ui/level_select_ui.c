#include "level_select_ui.h"
#include "../assets.h"
#include "common.h"

#include <stdio.h>

#define RR_UI_LEVEL_SELECT_PADDING 10

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
    rr_ui_active_element_group_uninit(&level_select->element_group);
}

void rr_ui_lecel_select_update_level_text_text(rrUILevelSelect* level_select) {
    char level_text[8];
    sprintf(level_text, "%d", level_select->current_level);
    rr_renderer_update_text_sprite(level_select->_renderer, level_select->level_text.sprite, RR_FONT_BUTTON, level_text);
}

void rr_ui_level_select_update_active_level_text(rrUILevelSelect* level_select) {
    if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_DOWN))
        rr_ui_active_element_group_next(&level_select->element_group);
    else if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_LEFT)) {
        level_select->current_level -= 1;

        if (level_select->current_level < 1)
            level_select->current_level = rr_game_get_level_count(level_select->_game);

        rr_ui_lecel_select_update_level_text_text(level_select);
    }
    else if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_RIGHT)) {
        level_select->current_level += 1;

        if (level_select->current_level > rr_game_get_level_count(level_select->_game))
            level_select->current_level = 1;

        rr_ui_lecel_select_update_level_text_text(level_select);
    }
}

void rr_ui_level_select_update_active_buttons(rrUILevelSelect* level_select) {
    if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_RIGHT))
        rr_ui_active_element_group_next(&level_select->element_group);
    else if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_LEFT))
        rr_ui_active_element_group_previous(&level_select->element_group);
    else if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_UP))
        rr_ui_active_element_group_set(&level_select->element_group, 0);
}


void rr_ui_level_select_update(rrUILevelSelect* level_select) {
    if (level_select->level_text.element.active)
        rr_ui_level_select_update_active_level_text(level_select);
    else
        rr_ui_level_select_update_active_buttons(level_select);
}

void rr_ui_level_select_center_content(rrUILevelSelect* level_select, rrRect* content_rect) {
    rrPoint screen_size, offset;
    rr_renderer_get_screen_size(level_select->_renderer, &screen_size);
    level_select->layout_rect.w = content_rect->w + RR_UI_LEVEL_SELECT_PADDING * 2;
    level_select->layout_rect.h = content_rect->h + RR_UI_LEVEL_SELECT_PADDING * 2;
    level_select->layout_rect.x = screen_size.x / 2 - level_select->layout_rect.w / 2;
    level_select->layout_rect.y = screen_size.y / 2 - level_select->layout_rect.h / 2;

    rr_point_set(&offset, level_select->layout_rect.x + RR_UI_LEVEL_SELECT_PADDING, level_select->layout_rect.y + RR_UI_LEVEL_SELECT_PADDING);

    level_select->bar.rect.x += level_select->layout_rect.x + 4;
    level_select->bar.rect.y += level_select->layout_rect.y + 5;
    level_select->bar.rect.w = level_select->layout_rect.w - 8;

    rr_point_add_and_assign(&level_select->level_prompt.element.position, &offset);
    rr_point_add_and_assign(&level_select->level_text.element.position, &offset);
    rr_point_add_and_assign(&level_select->ok_button.element.position, &offset);
    rr_point_add_and_assign(&level_select->cancel_button.element.position, &offset);
}

void rr_ui_level_select_layout(rrUILevelSelect* level_select) {
    rrSprite* text_sprite;
    rrPoint position = {0, 0};
    rrPoint button_size;
    char text_buffer[64];
    rrColor color;
    rrRect element_rect;
    rrRect content_rect = {0, 0, 0, 10};

    rr_color_set(&color, 0, 0, 130, 255);
    rr_ui_basic_rect_init(&level_select->bar, &content_rect, &color, level_select->_renderer); /* width will be set below */
    position.y = content_rect.h;

    rr_color_black(&color);

    sprintf(text_buffer, "Select a level (1-%d): ", rr_game_get_level_count(level_select->_game));
    text_sprite = rr_renderer_create_text(level_select->_renderer, RR_FONT_BUTTON, text_buffer);
    rr_renderer_set_sprite_tint_color(level_select->_renderer, text_sprite, &color);
    rr_ui_basic_sprite_init(&level_select->level_prompt, &position, text_sprite, level_select->_renderer);
    rr_ui_basic_sprite_get_rect(&level_select->level_prompt, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);
    position.y += text_sprite->rect.h + 10;

    sprintf(text_buffer, "%d ", level_select->current_level);
    text_sprite = rr_renderer_create_text(level_select->_renderer, RR_FONT_BUTTON, text_buffer);
    level_select->level_text.element.active = 1;
    rr_renderer_set_sprite_tint_color(level_select->_renderer, text_sprite, &color);
    rr_ui_basic_sprite_init(&level_select->level_text, &position, text_sprite, level_select->_renderer);
    rr_ui_active_element_group_add(&level_select->element_group, &level_select->level_text.element);
    rr_ui_basic_sprite_get_rect(&level_select->level_prompt, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);
    position.y += text_sprite->rect.h + 10;

    rr_ui_button_init(&level_select->ok_button, level_select->_renderer, "Ok", &position);
    rr_ui_button_get_size(&level_select->ok_button, &button_size);
    rr_ui_active_element_group_add(&level_select->element_group, &level_select->ok_button.element);
    rr_ui_basic_sprite_get_rect(&level_select->level_prompt, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);
    position.x += button_size.x + 10;

    rr_ui_button_init(&level_select->cancel_button, level_select->_renderer, "Cancel", &position);
    rr_ui_button_get_rect(&level_select->cancel_button, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);

    level_select->bar.rect.w = content_rect.w;

    rr_ui_active_element_group_add(&level_select->element_group, &level_select->cancel_button.element);

    rr_ui_level_select_center_content(level_select, &content_rect);
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
    rrColor color;
    rr_color_black(&color);

    rr_ui_draw_inset_rect(level_select->_renderer, &level_select->layout_rect);
    rr_renderer_color(level_select->_renderer, &color);
    rr_renderer_draw_rect(level_select->_renderer, &level_select->layout_rect);

    rr_ui_basic_rect_draw(&level_select->bar);
    rr_ui_basic_sprite_draw(&level_select->level_prompt);
    rr_level_select_draw_level_text(level_select);

    rr_ui_button_draw(&level_select->ok_button);
    rr_ui_button_draw(&level_select->cancel_button);
}
