#ifndef RR_UI_ONSCREEN_KEYBOARD_H
#define RR_UI_ONSCREEN_KEYBOARD_H

#include "../draw/renderer.h"
#include "../input.h"

typedef struct {
    rrRenderer* _renderer;
    rrInput* _input;

    rrSprite* _lower_key_sprites[40];
    rrSprite* _upper_key_sprites[40];
    rrSprite** _active_sprites;
    int _active_index;

    char _lower_key_values[40];
    char _upper_key_values[40];

    rrPoint _content_pos;

} rrUiOnscreenKeyboard;

rrUiOnscreenKeyboard* rr_ui_onscreen_keyboard_create(rrRenderer* renderer, rrInput* input);
void rr_ui_onscreen_keyboard_destroy(rrUiOnscreenKeyboard* keyboard);
void rr_ui_onscreen_keyboard_update(rrUiOnscreenKeyboard* keyboard);

void rr_ui_onscreen_keyboard_draw(rrUiOnscreenKeyboard* keyboard);
void rr_ui_onscreen_keyboard_get_rect(rrUiOnscreenKeyboard* keyboard, rrRect* rect);


#endif
