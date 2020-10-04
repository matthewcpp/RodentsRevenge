#include "text_input.h"

void rr_text_input_init(rrTextInput* text_input, rrInput* input,  rrRenderer* renderer) {
    text_input->_input = input;
    text_input->_renderer = renderer;
    text_input->_strbuf = cutil_strbuf_create();
}

void rr_text_input_uninit(rrTextInput* text_input) {
    cutil_strbuf_destroy(text_input->_strbuf);
}

void rr_text_input_update(rrTextInput* textInput) {

}

void rr_text_input_draw(rrTextInput* textInput) {

}
