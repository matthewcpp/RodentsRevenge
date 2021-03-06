#ifndef RR_INPUT_H
#define RR_INPUT_H

#include "point.h"

typedef enum{
    RR_INPUT_BUTTON_UP,
    RR_INPUT_BUTTON_DOWN,
    RR_INPUT_BUTTON_LEFT,
    RR_INPUT_BUTTON_RIGHT,
    RR_INPUT_BUTTON_START,
    RR_INPUT_BUTTON_ACCEPT,
    RR_INPUT_BUTTON_BACK,
    RR_INPUT_BUTTON_COUNT
} rrInputButton;

typedef struct rrInput rrInput;

/** Returns non zero value if the supplied key was pressed this update frame. */
int rr_input_button_down(rrInput* input, rrInputButton button);

/** Returns the amount of time the supplied button has been held down.
 * Note if a button was just pressed this frame, its held time will be zero.
 */
int rr_input_button_held_time(rrInput* input, rrInputButton button);

int rr_input_pointer_down(rrInput* input);
int rr_input_pointer_up(rrInput* input);
void rr_input_pointer_pos(rrInput* input, rrPoint* pos);


#endif
