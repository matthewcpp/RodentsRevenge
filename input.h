#ifndef RR_INPUT_H
#define RR_INPUT_H

typedef enum{
    RR_INPUT_BUTTON_UP,
    RR_INPUT_BUTTON_DOWN,
    RR_INPUT_BUTTON_LEFT,
    RR_INPUT_BUTTON_RIGHT,
    RR_INPUT_BUTTON_COUNT
} rrInputButton;

typedef struct rrInput rrInput;

int rr_input_button_down(rrInput* input, rrInputButton button);


#endif
