#include "onscreen_keyboard.h"

#include "../assets.h"

#include <stdio.h>
#include <stdlib.h>

void rr_ui_onscreen_keyboard_init_keys(rrUiOnscreenKeyboard* onscreen_keyboard) {
    int i;
    char buf[8];
    rrColor black;
    rr_color_black(&black);

    onscreen_keyboard->_key_sprites = calloc(10, sizeof(rrSprite*));

    for (i = 0; i < 10; i++) {
        rrSprite* key_sprite;
        sprintf(buf, "%d", i);
        key_sprite = rr_renderer_create_text(onscreen_keyboard->_renderer, RR_FONT_KEYBOARD, buf);
        rr_renderer_set_sprite_tint_color(onscreen_keyboard->_renderer, key_sprite, &black);
        onscreen_keyboard->_key_sprites[i] = key_sprite;
    }
}

rrUiOnscreenKeyboard* rr_ui_onscreen_keyboard_create(rrRenderer* renderer){
    rrUiOnscreenKeyboard* onscreen_keyboard = malloc(sizeof(rrUiOnscreenKeyboard));

    onscreen_keyboard->_renderer = renderer;
    rr_ui_onscreen_keyboard_init_keys(onscreen_keyboard);

    return onscreen_keyboard;
}

void rr_ui_onscreen_keyboard_destroy(rrUiOnscreenKeyboard* onscreen_keyboard) {
    free(onscreen_keyboard->_key_sprites);
    free(onscreen_keyboard);
}

#define KEY_WIDTH 40
#define KEY_HEIGHT 40

void rr_ui_onscreen_keyboard_draw(rrUiOnscreenKeyboard* onscreen_keyboard) {
    rrColor white, black;
    rrRect key_rect = {5, 430, KEY_WIDTH, KEY_HEIGHT};
    int i;

    rr_color_white(&white);
    rr_color_black(&black);

    for (i = 0; i < 10; i++) {
        rrPoint spritePos;
        rrSprite* text = onscreen_keyboard->_key_sprites[i];
        spritePos.x = key_rect.x + (KEY_WIDTH / 2) - (text->rect.w / 2);
        spritePos.y = key_rect.y + (KEY_HEIGHT / 2) - (text->rect.h / 2);

        rr_renderer_color(onscreen_keyboard->_renderer, &white);
        rr_renderer_fill_rect(onscreen_keyboard->_renderer, &key_rect);

        rr_renderer_color(onscreen_keyboard->_renderer, &black);
        rr_renderer_draw_rect(onscreen_keyboard->_renderer, &key_rect);

        rr_renderer_draw_sprite(onscreen_keyboard->_renderer, text, &spritePos);

        key_rect.x += KEY_WIDTH + 5;
    }
}
