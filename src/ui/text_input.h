#ifndef RR_TEXT_INPUT_H
#define RR_TEXT_INPUT_H

#include "cutil/strbuf.h"

#include "../input.h"
#include "../draw/renderer.h"

#include "basic_sprite.h"

typedef struct {
    rrInput* _input;
    rrRenderer* _renderer;
    cutil_strbuf* _strbuf;
} rrTextInput;

void rr_text_input_init(rrTextInput* text_input, rrInput* input,  rrRenderer* renderer);
void rr_text_input_uninit(rrTextInput* text_input);
void rr_text_input_update(rrTextInput* textInput);
void rr_text_input_draw(rrTextInput* textInput);

#endif
