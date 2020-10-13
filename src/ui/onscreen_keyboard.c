#include "onscreen_keyboard.h"

#include "../assets.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    KEY_VALUE_SPACE = 32,
    KEY_VALUE_DEL = 8,
    KEY_VAL_UPPER,
    KEY_VAL_DONE
} rrKeyVal;

void rr_ui_onscreen_keyboard_init_keys(rrUiOnscreenKeyboard* keyboard);
void rr_ui_onscreen_keyboard_calculate_content_pos(rrUiOnscreenKeyboard* keyboard);
int rr_ui_onscreen_keyboard_set_active_key(rrUiOnscreenKeyboard* keyboard, int index);

rrUiOnscreenKeyboard* rr_ui_onscreen_keyboard_create(rrRenderer* renderer, rrInput* input){
    rrUiOnscreenKeyboard* keyboard = malloc(sizeof(rrUiOnscreenKeyboard));

    keyboard->_renderer = renderer;
    keyboard->_input = input;
    rr_ui_onscreen_keyboard_calculate_content_pos(keyboard);
    rr_ui_onscreen_keyboard_init_keys(keyboard);
    keyboard->_active_sprites = keyboard->_lower_key_sprites;
    keyboard->_active_index = 0;
    rr_ui_onscreen_keyboard_set_active_key(keyboard, keyboard->_active_index);

    keyboard->on_char = NULL;
    keyboard->on_backspace = NULL;
    keyboard->on_done = NULL;
    keyboard ->user_data = NULL;

    return keyboard;
}

void rr_ui_onscreen_keyboard_destroy(rrUiOnscreenKeyboard* keyboard) {
    free(keyboard->_lower_key_sprites);
    free(keyboard);
}

void rr_ui_onscreen_keyboard_show(rrUiOnscreenKeyboard* keyboard) {
    keyboard->_active_sprites = &keyboard->_lower_key_sprites[0];
    rr_ui_onscreen_keyboard_set_active_key(keyboard, 0);
    keyboard->active = 1;
}

/** Updates the color the active key.  Assumes index is valid. */
int rr_ui_onscreen_keyboard_set_active_key(rrUiOnscreenKeyboard* keyboard, int index) {
    rrColor color;
    rr_color_black(&color);

    if (index < 0 || index >= 40)
        return 0;

    rr_renderer_set_sprite_tint_color(keyboard->_renderer, keyboard->_lower_key_sprites[keyboard->_active_index], &color);
    rr_renderer_set_sprite_tint_color(keyboard->_renderer, keyboard->_upper_key_sprites[keyboard->_active_index], &color);

    keyboard->_active_index = index;
    rr_color_white(&color);

    rr_renderer_set_sprite_tint_color(keyboard->_renderer, keyboard->_lower_key_sprites[keyboard->_active_index], &color);
    rr_renderer_set_sprite_tint_color(keyboard->_renderer, keyboard->_upper_key_sprites[keyboard->_active_index], &color);

    return 1;
}

#define KEY_WIDTH 25
#define KEY_HEIGHT 25
#define KEY_MARGIN_W 4
#define KEY_MARGIN_H 4


static void calculate_key_rect_pos(rrUiOnscreenKeyboard* keyboard, int index, rrRect* rect) {
    rect->x = (index % 10 * (KEY_WIDTH + KEY_MARGIN_W)) + keyboard->_content_pos.x;
    rect->y = (index / 10 * (KEY_HEIGHT + KEY_MARGIN_H)) + keyboard->_content_pos.y;
}

static void calculate_key_sprite_pos(rrRect* key_rect, rrSprite* sprite, rrPoint* sprite_pos) {
    sprite_pos->x = key_rect->x + (KEY_WIDTH / 2) - (sprite->rect.w / 2);
    sprite_pos->y = key_rect->y + (KEY_HEIGHT / 2) - (sprite->rect.h / 2);
}

void rr_ui_onscreen_keyboard_draw_key(rrUiOnscreenKeyboard* keyboard, int index, rrColor* background_color) {
    rrSprite* text = keyboard->_active_sprites[index];
    rrPoint sprite_pos;
    rrRect key_rect;
    rrColor black;
    rr_color_black(&black);
    rr_rect_set_size(&key_rect, KEY_WIDTH, KEY_HEIGHT);

    calculate_key_rect_pos(keyboard, index, &key_rect);
    calculate_key_sprite_pos(&key_rect, text, &sprite_pos);

    rr_renderer_color(keyboard->_renderer, background_color);
    rr_renderer_fill_rect(keyboard->_renderer, &key_rect);

    rr_renderer_color(keyboard->_renderer, &black);
    rr_renderer_draw_rect(keyboard->_renderer, &key_rect);

    rr_renderer_draw_sprite(keyboard->_renderer, text, &sprite_pos);
}

void rr_ui_onscreen_keyboard_draw(rrUiOnscreenKeyboard* keyboard) {
    rrColor background;
    int i;
    rr_color_white(&background);

    for (i = 0; i < 40; i++) {
        rr_ui_onscreen_keyboard_draw_key(keyboard, i, &background);
    }

    /* note that active keys will draw over existing key */
    rr_color_blue(&background);
    rr_ui_onscreen_keyboard_draw_key(keyboard, keyboard->_active_index, &background);
}

void rr_ui_onscreen_keyboard_create_key_sprites(rrRenderer* renderer, char* values, rrSprite** sprites, int count) {
    rrColor black = {0, 0, 0, 0};
    rrSprite* key_sprite;
    char buf[8];
    int i;

    for (i = 0; i < count; i++) {
        sprintf(buf, "%c", values[i]);
        key_sprite = rr_renderer_create_text(renderer, RR_FONT_KEYBOARD, buf);
        rr_renderer_set_sprite_tint_color(renderer, key_sprite, &black);
        sprites[i] = key_sprite;
    }
}

void rr_ui_onscreen_keyboard_create_special_keys(rrRenderer* renderer, char* values, rrSprite** sprites) {
    const char* special_keys[4] = {"SP", "DEL", "SH", "END"};
    char special_values[4] = {KEY_VALUE_SPACE, KEY_VALUE_DEL, KEY_VAL_UPPER, KEY_VAL_DONE};
    rrColor black = {0, 0, 0, 0};
    rrSprite* key_sprite;
    char buf[8];
    int i;

    for (i = 0; i < 4; i++) {
        sprintf(buf, "%s", special_keys[i]);
        key_sprite = rr_renderer_create_text(renderer, RR_FONT_KEYBOARD, buf);
        rr_renderer_set_sprite_tint_color(renderer, key_sprite, &black);

        values[36 + i] = special_values[i];
        sprites[36 + i] = key_sprite;
    }
}

void rr_ui_onscreen_keyboard_init_keys(rrUiOnscreenKeyboard* keyboard) {
    int index = 0;
    char ch;

    for (ch = '0'; ch <= '9'; ch++) {
        keyboard->_lower_key_values[index] = ch;
        keyboard->_upper_key_values[index++] = ch;
    }

    for (ch = 'A'; ch <= 'Z'; ch ++) {
        keyboard->_upper_key_values[index] = ch;
        keyboard->_lower_key_values[index++] = ch + 32;
    }

    rr_ui_onscreen_keyboard_create_key_sprites(keyboard->_renderer, keyboard->_lower_key_values, keyboard->_lower_key_sprites, 36);
    rr_ui_onscreen_keyboard_create_special_keys(keyboard->_renderer, keyboard->_lower_key_values, keyboard->_lower_key_sprites);

    rr_ui_onscreen_keyboard_create_key_sprites(keyboard->_renderer, keyboard->_upper_key_values, keyboard->_upper_key_sprites, 36);
    rr_ui_onscreen_keyboard_create_special_keys(keyboard->_renderer, keyboard->_upper_key_values, keyboard->_upper_key_sprites);
}

void rr_ui_onscreen_keyboard_calculate_content_pos(rrUiOnscreenKeyboard* keyboard) {
    rrPoint screen_size;
    int keyboard_width = KEY_WIDTH * 10 + KEY_MARGIN_W * 8;
    int keyboard_height = KEY_HEIGHT * 4 + KEY_MARGIN_H * 3;

    rr_renderer_get_screen_size(keyboard->_renderer, &screen_size);

    keyboard->_content_pos.x = screen_size.x / 2 - keyboard_width / 2;
    keyboard->_content_pos.y = screen_size.y - KEY_MARGIN_W - keyboard_height;
}

void rr_ui_onscreen_keyboard_activate_key(rrUiOnscreenKeyboard* keyboard) {
    int value = keyboard->_active_sprites == keyboard->_lower_key_sprites ?
            keyboard->_lower_key_values[keyboard->_active_index] : keyboard->_upper_key_values[keyboard->_active_index];

    switch (value) {
        case KEY_VALUE_DEL: {
            if (keyboard->on_backspace)
                keyboard->on_backspace(keyboard->user_data);
        }
        break;

        case KEY_VAL_DONE: {
            if (keyboard->on_done)
                keyboard->on_done(keyboard->user_data);
        }
        break;

        case KEY_VAL_UPPER: {
            if (keyboard->_active_sprites == keyboard->_lower_key_sprites)
                keyboard->_active_sprites = keyboard->_upper_key_sprites;
            else
                keyboard->_active_sprites = keyboard->_lower_key_sprites;
        }
        break;

        default: {
            if (keyboard->on_char)
                keyboard->on_char(value, keyboard->user_data);

            keyboard->_active_sprites = keyboard->_lower_key_sprites;
        }
    }
}

void rr_ui_onscreen_keyboard_update_input(rrUiOnscreenKeyboard* keyboard) {
    if (rr_input_button_down(keyboard->_input, RR_INPUT_BUTTON_RIGHT))
        rr_ui_onscreen_keyboard_set_active_key(keyboard, keyboard->_active_index + 1);

    if (rr_input_button_down(keyboard->_input, RR_INPUT_BUTTON_LEFT))
        rr_ui_onscreen_keyboard_set_active_key(keyboard, keyboard->_active_index - 1);

    if (rr_input_button_down(keyboard->_input, RR_INPUT_BUTTON_DOWN))
        rr_ui_onscreen_keyboard_set_active_key(keyboard, keyboard->_active_index + 10);

    if (rr_input_button_down(keyboard->_input, RR_INPUT_BUTTON_UP))
        rr_ui_onscreen_keyboard_set_active_key(keyboard, keyboard->_active_index - 10);

    if (rr_input_button_down(keyboard->_input, RR_INPUT_BUTTON_ACCEPT))
        rr_ui_onscreen_keyboard_activate_key(keyboard);
}

void rr_ui_onscreen_keyboard_update_pointer(rrUiOnscreenKeyboard* keyboard) {
    rrRect keyboard_rect;
    rrPoint pointer_pos;
    rrPoint selected_key = {-1, -1};
    int i;

    if (!rr_input_pointer_up((keyboard->_input)))
        return;

    rr_ui_onscreen_keyboard_get_rect(keyboard, &keyboard_rect);
    rr_input_pointer_pos(keyboard->_input, &pointer_pos);

    if (!rr_rect_contains_point(&keyboard_rect, pointer_pos.x, pointer_pos.y))
        return;

    rr_point_sub(&pointer_pos, &pointer_pos, &keyboard->_content_pos);

    for (i = 0; i < 10; i++) {
        if (pointer_pos.x < KEY_WIDTH) {
            selected_key.x = i;
            break;
        }

        pointer_pos.x -= KEY_WIDTH;

        if (pointer_pos.x < KEY_MARGIN_W)
            return;

        pointer_pos.x -= KEY_MARGIN_W;
    }

    if (selected_key.x == -1)
        return;

    for (i = 0; i < 4; i++) {
        if (pointer_pos.y < KEY_HEIGHT) {
            selected_key.y = i;
            break;
        }

        pointer_pos.y -= KEY_HEIGHT;

        if (pointer_pos.y < KEY_MARGIN_H)
            return;

        pointer_pos.y -= KEY_MARGIN_H;
    }

    if (selected_key.y == -1)
        return;

    rr_ui_onscreen_keyboard_set_active_key(keyboard, selected_key.x % 10 + selected_key.y * 10);
    rr_ui_onscreen_keyboard_activate_key(keyboard);
}

void rr_ui_onscreen_keyboard_update(rrUiOnscreenKeyboard* keyboard) {
    rr_ui_onscreen_keyboard_update_input(keyboard);
    rr_ui_onscreen_keyboard_update_pointer(keyboard);
}

void rr_ui_onscreen_keyboard_get_rect(rrUiOnscreenKeyboard* keyboard, rrRect* rect) {
    rect->x = keyboard->_content_pos.x;
    rect->y = keyboard->_content_pos.y;
    rect->w = KEY_WIDTH * 10 + KEY_MARGIN_H * 9;
    rect->h = KEY_HEIGHT * 4 + KEY_MARGIN_H * 3;
}
