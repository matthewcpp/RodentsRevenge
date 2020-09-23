#ifndef RR_BUTTON_H
#define RR_BUTTON_H

#include "../draw/renderer.h"
#include "element.h"

typedef void (*rrUiButtonCallbackFunc)(void* data);

typedef struct {
    rrUiElement element;
    rrRenderer* _renderer;
    rrSprite* sprite;

    rrUiButtonCallbackFunc _callback;
    void* _user_data;
} rrUiButton;

void rr_ui_button_init(rrUiButton* button, rrRenderer* renderer, const char* text, rrPoint* pos);
void rr_ui_button_draw(rrUiButton* button);
void rr_ui_button_get_size(rrUiButton* button, rrPoint* size);
void rr_ui_button_set_callback(rrUiButton* button, rrUiButtonCallbackFunc callback, void* user_data);
void rr_ui_button_activate(rrUiButton* button);

#endif
