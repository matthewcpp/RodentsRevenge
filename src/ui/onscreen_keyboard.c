#include "onscreen_keyboard.h"

#include "../assets.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    KEY_VALUE_SPACE,
    KEY_VALUE_DEL,
    KEY_VAL_UPPER,
    KEY_VAL_DONE
} rrKeyVal;

void rr_ui_onscreen_keyboard_init_keys(rrUiOnscreenKeyboard* keyboard);
void rr_ui_onscreen_keyboard_calculate_content_pos(rrUiOnscreenKeyboard* keyboard);
void rr_ui_onscreen_keyboard_set_active_key(rrUiOnscreenKeyboard* keyboard, int index);

rrUiOnscreenKeyboard* rr_ui_onscreen_keyboard_create(rrRenderer* renderer){
    rrUiOnscreenKeyboard* keyboard = malloc(sizeof(rrUiOnscreenKeyboard));

    keyboard->_renderer = renderer;
    rr_ui_onscreen_keyboard_calculate_content_pos(keyboard);
    rr_ui_onscreen_keyboard_init_keys(keyboard);
    keyboard->_active_sprites = &keyboard->_lower_key_sprites[0];
    keyboard->_active_index = 0;
    rr_ui_onscreen_keyboard_set_active_key(keyboard, keyboard->_active_index);

    return keyboard;
}

void rr_ui_onscreen_keyboard_destroy(rrUiOnscreenKeyboard* keyboard) {
    free(keyboard->_lower_key_sprites);
    free(keyboard);
}

/** Updates the color the active key.  Assumes index is valid. */
void rr_ui_onscreen_keyboard_set_active_key(rrUiOnscreenKeyboard* keyboard, int index) {
    rrColor color;
    rr_color_black(&color);

    rr_renderer_set_sprite_tint_color(keyboard->_renderer, keyboard->_lower_key_sprites[keyboard->_active_index], &color);
    rr_renderer_set_sprite_tint_color(keyboard->_renderer, keyboard->_upper_key_sprites[keyboard->_active_index], &color);

    keyboard->_active_index = index;
    rr_color_white(&color);

    rr_renderer_set_sprite_tint_color(keyboard->_renderer, keyboard->_lower_key_sprites[keyboard->_active_index], &color);
    rr_renderer_set_sprite_tint_color(keyboard->_renderer, keyboard->_upper_key_sprites[keyboard->_active_index], &color);
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
    rrPoint sprite_pos;
    rrRect key_rect;
    rrColor black;
    rr_color_black(&black);
    rr_rect_set_size(&key_rect, KEY_WIDTH, KEY_HEIGHT);
    calculate_key_rect_pos(keyboard, index, &key_rect);

    rrSprite* text = keyboard->_active_sprites[index];
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
        keyboard->_lower_key_values[index] = ch;
        keyboard->_upper_key_values[index++] = ch + 32;
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
