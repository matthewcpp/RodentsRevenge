#ifndef RR_UI_ONSCREEN_KEYBOARD_H
#define RR_UI_ONSCREEN_KEYBOARD_H

#include "../draw/renderer.h"

typedef struct {
    rrRenderer* _renderer;
    rrSprite** _key_sprites;

} rrUiOnscreenKeyboard;

rrUiOnscreenKeyboard* rr_ui_onscreen_keyboard_create(rrRenderer* renderer);
void rr_ui_onscreen_keyboard_destroy(rrUiOnscreenKeyboard* onscreen_keyboard);

void rr_ui_onscreen_keyboard_draw(rrUiOnscreenKeyboard* onscreen_keyboard);


#endif
