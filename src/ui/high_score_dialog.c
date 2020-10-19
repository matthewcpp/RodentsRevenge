#include "high_score_dialog.h"
#include "common.h"
#include "../assets.h"

#include <stdlib.h>

static void rr_ui_high_score_dialog_layout(rrUiHighScoreDialog* high_score_dialog);
static void on_name_entered(void* user_data);

rrUiHighScoreDialog* rr_ui_high_score_dialog_create(rrRenderer* renderer, rrInput* input) {
    rrUiHighScoreDialog* high_score_dialog = malloc(sizeof(rrUiHighScoreDialog));

    high_score_dialog->_renderer = renderer;
    high_score_dialog->_input = input;
    high_score_dialog->active = 0;

    rr_ui_active_element_group_init(&high_score_dialog->_element_group);
    rr_ui_high_score_dialog_layout(high_score_dialog);

    high_score_dialog->text_input.text_entered_callback = on_name_entered;
    high_score_dialog->text_input.callback_data = high_score_dialog;

    return high_score_dialog;
}

void rr_ui_high_score_dialog_delete(rrUiHighScoreDialog* high_score_dialog) {
    rr_ui_active_element_group_uninit(&high_score_dialog->_element_group);

    free(high_score_dialog);
}

#define RR_UI_SCORE_DIALOG_PADDING 10

void rr_ui_high_score_dialog_center_content(rrUiHighScoreDialog* high_score_dialog, rrRect* content_rect) {
    rrPoint screen_size, offset;
    rr_renderer_get_screen_size(high_score_dialog->_renderer, &screen_size);
    high_score_dialog->_layout_rect.w = content_rect->w + RR_UI_SCORE_DIALOG_PADDING * 2;
    high_score_dialog->_layout_rect.h = content_rect->h + RR_UI_SCORE_DIALOG_PADDING * 2;
    high_score_dialog->_layout_rect.x = screen_size.x / 2 - high_score_dialog->_layout_rect.w / 2;
    high_score_dialog->_layout_rect.y = screen_size.y / 2 - high_score_dialog->_layout_rect.h / 2;

    rr_point_set(&offset, high_score_dialog->_layout_rect.x + RR_UI_SCORE_DIALOG_PADDING, high_score_dialog->_layout_rect.y + RR_UI_SCORE_DIALOG_PADDING);
    high_score_dialog->bar.rect.x += high_score_dialog->_layout_rect.x + 4;
    high_score_dialog->bar.rect.y += high_score_dialog->_layout_rect.y + 5;
    high_score_dialog->bar.rect.w = high_score_dialog->_layout_rect.w - 8;

    rr_point_add_and_assign(&high_score_dialog->text_input.element.position, &offset);
    high_score_dialog->text_input.width_override = content_rect->w - (2* RR_UI_SCORE_DIALOG_PADDING);
    rr_point_add_and_assign(&high_score_dialog->ok_button.element.position, &offset);
    rr_point_add_and_assign(&high_score_dialog->prompt_line_1.element.position, &offset);
    rr_point_add_and_assign(&high_score_dialog->prompt_line_2.element.position, &offset);
}

void rr_ui_high_score_dialog_layout(rrUiHighScoreDialog* high_score_dialog) {
    rrRect element_rect;
    rrRect content_rect = {0, 0, 0, 10};
    rrColor color;
    rrPoint position = {0, 0};
    rrSprite* text;

    rr_color_set(&color, 0, 0, 130, 255);
    rr_ui_basic_rect_init(&high_score_dialog->bar, &content_rect, &color, high_score_dialog->_renderer); /* width will be set below */
    position.y = content_rect.h;

    rr_color_black(&color);
    text = rr_renderer_create_text(high_score_dialog->_renderer, RR_FONT_BUTTON, "You have achieved a high score!");
    rr_renderer_set_sprite_tint_color(high_score_dialog->_renderer, text, &color);
    rr_ui_basic_sprite_init(&high_score_dialog->prompt_line_1, &position, text, high_score_dialog->_renderer);
    rr_ui_basic_sprite_get_rect(&high_score_dialog->prompt_line_1, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);
    position.y += element_rect.h + 5;

    text = rr_renderer_create_text(high_score_dialog->_renderer, RR_FONT_BUTTON, "Please enter your name:");
    rr_renderer_set_sprite_tint_color(high_score_dialog->_renderer, text, &color);
    rr_ui_basic_sprite_init(&high_score_dialog->prompt_line_2, &position, text, high_score_dialog->_renderer);
    rr_ui_basic_sprite_get_rect(&high_score_dialog->prompt_line_2, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);
    position.y += element_rect.h + 5;

    rr_ui_text_input_init(&high_score_dialog->text_input, high_score_dialog->_input, high_score_dialog->_renderer, &position);
    rr_ui_text_input_get_rect(&high_score_dialog->text_input, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);
    position.y += element_rect.h + 10;

    rr_ui_button_init(&high_score_dialog->ok_button, high_score_dialog->_renderer, "Ok", &position);
    rr_ui_button_get_rect(&high_score_dialog->ok_button, &element_rect);
    rr_rect_encapsulate(&content_rect, &element_rect);

    rr_ui_active_element_group_add(&high_score_dialog->_element_group, &high_score_dialog->text_input.element);
    rr_ui_active_element_group_add(&high_score_dialog->_element_group, &high_score_dialog->ok_button.element);

    rr_ui_high_score_dialog_center_content(high_score_dialog, &content_rect);
}

void rr_ui_high_score_dialog_show(rrUiHighScoreDialog* high_score_dialog) {
    high_score_dialog->active = 1;
    rr_ui_text_input_show(&high_score_dialog->text_input);
}

void rr_ui_high_score_dialog_update(rrUiHighScoreDialog* high_score_dialog) {
    rr_ui_text_input_update(&high_score_dialog->text_input);

    if (high_score_dialog->text_input.onscreen_keyboard->active)
        return;

    if (rr_input_button_down(high_score_dialog->_input, RR_INPUT_BUTTON_DOWN))
        rr_ui_active_element_group_next(&high_score_dialog->_element_group);
    else if (rr_input_button_down(high_score_dialog->_input, RR_INPUT_BUTTON_UP))
        rr_ui_active_element_group_previous(&high_score_dialog->_element_group);

    if (rr_input_pointer_up(high_score_dialog->_input)) {
        rrPoint pos;
        rr_input_pointer_pos(high_score_dialog->_input, &pos);
        rr_ui_button_try_click(&high_score_dialog->ok_button, &pos);
    }

    if ((rr_input_button_down(high_score_dialog->_input, RR_INPUT_BUTTON_ACCEPT) || rr_input_button_down(high_score_dialog->_input, RR_INPUT_BUTTON_START)) && high_score_dialog->ok_button.element.active)
        rr_ui_button_activate(&high_score_dialog->ok_button);
}

void rr_ui_high_score_dialog_draw(rrUiHighScoreDialog* high_score_dialog) {
    rrColor color;
    rr_color_black(&color);

    rr_ui_draw_inset_rect(high_score_dialog->_renderer, &high_score_dialog->_layout_rect);
    rr_renderer_color(high_score_dialog->_renderer, &color);
    rr_renderer_draw_rect(high_score_dialog->_renderer, &high_score_dialog->_layout_rect);
    rr_ui_basic_rect_draw(&high_score_dialog->bar);
    rr_ui_basic_sprite_draw(&high_score_dialog->prompt_line_1);
    rr_ui_basic_sprite_draw(&high_score_dialog->prompt_line_2);
    rr_ui_button_draw(&high_score_dialog->ok_button);
    rr_ui_text_input_draw(&high_score_dialog->text_input);
}

static void on_name_entered(void* user_data) {
    rrUiHighScoreDialog* high_score_dialog = (rrUiHighScoreDialog*)user_data;
    rr_ui_active_element_group_next(&high_score_dialog->_element_group);
}
