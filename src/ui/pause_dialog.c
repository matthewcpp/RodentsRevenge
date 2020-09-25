#include "pause_dialog.h"
#include "../assets.h"
#include "common.h"

#define RR_UI_LEVEL_SELECT_PADDING 10

void rr_ui_pause_dialog_layout(rrUIPauseDialog * pause_dialog);

rrUIPauseDialog* rr_ui_pause_dialog_create(rrRenderer* renderer, rrInput* input) {
    rrUIPauseDialog* level_select = malloc(sizeof(rrUIPauseDialog));

    level_select->_input = input;
    level_select->_renderer = renderer;
    level_select->active = 0;

    rr_ui_active_element_group_init(&level_select->element_group);
    rr_ui_pause_dialog_layout(level_select);

    return level_select;
}

void rr_ui_pause_dialog_destroy(rrUIPauseDialog* pause_dialog) {
    rr_ui_active_element_group_uninit(&pause_dialog->element_group);
    free(pause_dialog);
}

void rr_ui_pause_dialog_update(rrUIPauseDialog* level_select) {
    if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_RIGHT))
        rr_ui_active_element_group_next(&level_select->element_group);
    else if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_LEFT))
        rr_ui_active_element_group_previous(&level_select->element_group);
    else if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_ACCEPT)) {
        rrUiButton * button = (rrUiButton*)rr_ui_active_element_group_get_current(&level_select->element_group);
        rr_ui_button_activate(button);
    }
    else if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_START))
        rr_ui_button_activate(&level_select->ok_button);
    else if (rr_input_button_down(level_select->_input, RR_INPUT_BUTTON_BACK))
        rr_ui_button_activate(&level_select->cancel_button);
}

void rr_ui_pause_dialog_on_resume(void* user_data) {
    rrUIPauseDialog* pause_dialog = (rrUIPauseDialog*)user_data;
    pause_dialog->active = 0;
}

void rr_ui_pause_dialog_center_content(rrUIPauseDialog* level_select, rrRect* content_rect) {
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
    rr_point_add_and_assign(&level_select->ok_button.element.position, &offset);
    rr_point_add_and_assign(&level_select->cancel_button.element.position, &offset);
}

void rr_ui_pause_dialog_layout(rrUIPauseDialog* level_select) {
    rrSprite* text_sprite;
    rrPoint position = {0, 0};
    rrPoint button_size;
    rrColor color;
    rrRect element_rect;
    rrRect content_rect = {0, 0, 0, 10};

    rr_color_set(&color, 0, 0, 130, 255);
    rr_ui_basic_rect_init(&level_select->bar, &content_rect, &color, level_select->_renderer); /* width will be set below */
    position.y = content_rect.h;

    rr_color_black(&color);

    text_sprite = rr_renderer_create_text(level_select->_renderer, RR_FONT_BUTTON, "Game Paused");
    rr_renderer_set_sprite_tint_color(level_select->_renderer, text_sprite, &color);
    rr_ui_basic_sprite_init(&level_select->level_prompt, &position, text_sprite, level_select->_renderer);
    rr_ui_basic_sprite_get_rect(&level_select->level_prompt, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);
    position.y += text_sprite->rect.h + 10;

    rr_ui_button_init(&level_select->ok_button, level_select->_renderer, "Resume", &position);
    rr_ui_button_set_callback(&level_select->ok_button, rr_ui_pause_dialog_on_resume, level_select);
    rr_ui_button_get_size(&level_select->ok_button, &button_size);
    rr_ui_active_element_group_add(&level_select->element_group, &level_select->ok_button.element);
    rr_ui_basic_sprite_get_rect(&level_select->level_prompt, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);
    position.x += button_size.x + 10;

    rr_ui_button_init(&level_select->cancel_button, level_select->_renderer, "Exit", &position);
    rr_ui_button_get_rect(&level_select->cancel_button, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);

    level_select->bar.rect.w = content_rect.w;

    rr_ui_active_element_group_add(&level_select->element_group, &level_select->cancel_button.element);

    rr_ui_pause_dialog_center_content(level_select, &content_rect);
}

void rr_ui_pause_dialog_draw(rrUIPauseDialog* level_select) {
    rrColor color;
    rr_color_black(&color);

    rr_ui_draw_inset_rect(level_select->_renderer, &level_select->layout_rect);
    rr_renderer_color(level_select->_renderer, &color);
    rr_renderer_draw_rect(level_select->_renderer, &level_select->layout_rect);

    rr_ui_basic_rect_draw(&level_select->bar);
    rr_ui_basic_sprite_draw(&level_select->level_prompt);

    rr_ui_button_draw(&level_select->ok_button);
    rr_ui_button_draw(&level_select->cancel_button);
}
