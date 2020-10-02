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

    rr_ui_active_element_group_init(&level_select->_element_group);
    rr_ui_pause_dialog_layout(level_select);

    return level_select;
}

void rr_ui_pause_dialog_destroy(rrUIPauseDialog* pause_dialog) {
    rr_ui_active_element_group_uninit(&pause_dialog->_element_group);
    free(pause_dialog);
}

void rr_ui_pause_dialog_show(rrUIPauseDialog* pause_dialog) {
    rr_ui_active_element_group_set(&pause_dialog->_element_group, 0);
    pause_dialog->active = 1;
}

void rr_ui_pause_dialog_update_input(rrUIPauseDialog* pause_dialog) {
    if (rr_input_button_down(pause_dialog->_input, RR_INPUT_BUTTON_RIGHT))
        rr_ui_active_element_group_next(&pause_dialog->_element_group);
    else if (rr_input_button_down(pause_dialog->_input, RR_INPUT_BUTTON_LEFT))
        rr_ui_active_element_group_previous(&pause_dialog->_element_group);
    else if (rr_input_button_down(pause_dialog->_input, RR_INPUT_BUTTON_ACCEPT)) {
        rrUiButton * button = (rrUiButton*)rr_ui_active_element_group_get_current(&pause_dialog->_element_group);
        rr_ui_button_activate(button);
    }
    else if (rr_input_button_down(pause_dialog->_input, RR_INPUT_BUTTON_START))
        rr_ui_button_activate(&pause_dialog->resume_button);
    else if (rr_input_button_down(pause_dialog->_input, RR_INPUT_BUTTON_BACK))
        rr_ui_button_activate(&pause_dialog->exit_button);
}

void rr_ui_pause_dialog_update_pointer(rrUIPauseDialog* pause_dialog) {
    rrPoint pos;

    if (!rr_input_pointer_up(pause_dialog->_input))
        return;

    rr_input_pointer_pos(pause_dialog->_input, &pos);

    rr_ui_button_try_click(&pause_dialog->resume_button, &pos);
    rr_ui_button_try_click(&pause_dialog->exit_button, &pos);
}

void rr_ui_pause_dialog_update(rrUIPauseDialog* pause_dialog) {
    rr_ui_pause_dialog_update_input(pause_dialog);
    rr_ui_pause_dialog_update_pointer(pause_dialog);
}

void rr_ui_pause_dialog_center_content(rrUIPauseDialog* pause_dialog, rrRect* content_rect) {
    rrPoint screen_size, offset;
    rr_renderer_get_screen_size(pause_dialog->_renderer, &screen_size);
    pause_dialog->_layout_rect.w = content_rect->w + RR_UI_LEVEL_SELECT_PADDING * 2;
    pause_dialog->_layout_rect.h = content_rect->h + RR_UI_LEVEL_SELECT_PADDING * 2;
    pause_dialog->_layout_rect.x = screen_size.x / 2 - pause_dialog->_layout_rect.w / 2;
    pause_dialog->_layout_rect.y = screen_size.y / 2 - pause_dialog->_layout_rect.h / 2;

    rr_point_set(&offset, pause_dialog->_layout_rect.x + RR_UI_LEVEL_SELECT_PADDING, pause_dialog->_layout_rect.y + RR_UI_LEVEL_SELECT_PADDING);

    pause_dialog->bar.rect.x += pause_dialog->_layout_rect.x + 4;
    pause_dialog->bar.rect.y += pause_dialog->_layout_rect.y + 5;
    pause_dialog->bar.rect.w = pause_dialog->_layout_rect.w - 8;

    rr_point_add_and_assign(&pause_dialog->level_prompt.element.position, &offset);
    rr_point_add_and_assign(&pause_dialog->resume_button.element.position, &offset);
    rr_point_add_and_assign(&pause_dialog->exit_button.element.position, &offset);
}

void rr_ui_pause_dialog_layout(rrUIPauseDialog* pause_dialog) {
    rrSprite* text_sprite;
    rrPoint position = {0, 0};
    rrPoint button_size;
    rrColor color;
    rrRect element_rect;
    rrRect content_rect = {0, 0, 0, 10};

    rr_color_set(&color, 0, 0, 130, 255);
    rr_ui_basic_rect_init(&pause_dialog->bar, &content_rect, &color, pause_dialog->_renderer); /* width will be set below */
    position.y = content_rect.h;

    rr_color_black(&color);

    text_sprite = rr_renderer_create_text(pause_dialog->_renderer, RR_FONT_BUTTON, "Game Paused");
    rr_renderer_set_sprite_tint_color(pause_dialog->_renderer, text_sprite, &color);
    rr_ui_basic_sprite_init(&pause_dialog->level_prompt, &position, text_sprite, pause_dialog->_renderer);
    rr_ui_basic_sprite_get_rect(&pause_dialog->level_prompt, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);
    position.y += text_sprite->rect.h + 10;

    rr_ui_button_init(&pause_dialog->resume_button, pause_dialog->_renderer, "Resume", &position);
    rr_ui_button_get_size(&pause_dialog->resume_button, &button_size);
    rr_ui_active_element_group_add(&pause_dialog->_element_group, &pause_dialog->resume_button.element);
    rr_ui_basic_sprite_get_rect(&pause_dialog->level_prompt, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);
    position.x += button_size.x + 10;

    rr_ui_button_init(&pause_dialog->exit_button, pause_dialog->_renderer, "Exit", &position);
    rr_ui_button_get_rect(&pause_dialog->exit_button, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);

    pause_dialog->bar.rect.w = content_rect.w;

    rr_ui_active_element_group_add(&pause_dialog->_element_group, &pause_dialog->exit_button.element);

    rr_ui_pause_dialog_center_content(pause_dialog, &content_rect);
}

void rr_ui_pause_dialog_draw(rrUIPauseDialog* pause_dialog) {
    rrColor color;
    rr_color_black(&color);

    rr_ui_draw_inset_rect(pause_dialog->_renderer, &pause_dialog->_layout_rect);
    rr_renderer_color(pause_dialog->_renderer, &color);
    rr_renderer_draw_rect(pause_dialog->_renderer, &pause_dialog->_layout_rect);

    rr_ui_basic_rect_draw(&pause_dialog->bar);
    rr_ui_basic_sprite_draw(&pause_dialog->level_prompt);

    rr_ui_button_draw(&pause_dialog->resume_button);
    rr_ui_button_draw(&pause_dialog->exit_button);
}
