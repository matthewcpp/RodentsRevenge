#ifndef RR_TEXT_INPUT_H
#define RR_TEXT_INPUT_H

#include "cutil/strbuf.h"

#include "../input.h"
#include "../draw/renderer.h"

#include "basic_sprite.h"

typedef struct {
    rrUiElement element;

    rrInput* _input;
    rrRenderer* _renderer;
    cutil_strbuf* strbuf;
    rrSprite* _text_sprite;

    int width_override;
} rrUiTextInput;

void rr_ui_text_input_init(rrUiTextInput* text_input, rrInput* input,  rrRenderer* renderer, rrPoint* pos, const char* str);
void rr_ui_text_input_uninit(rrUiTextInput* text_input);

/**
 * Call this when the strbuf has been updated in order to update the text sprite's rect and text sprite
 */
void rr_ui_text_input_buffer_updated(rrUiTextInput* text_input);
void rr_ui_text_input_draw(rrUiTextInput* text_input);
void rr_ui_text_input_get_rect(rrUiTextInput* text_input, rrRect* rect);

#endif
