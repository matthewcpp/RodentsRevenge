#include "clock.h"

#include <stdlib.h>
#include <string.h>


#define RR_CLOCK_TICK_SECONDS 1U
#define RR_CLOCK_TICK_MINUTES 2U
#define RR_CLOCK_TICK_TARGET 4U

void rr_clock_init(rrClock* clock) {
    rr_clock_reset(clock);
}

void rr_clock_reset(rrClock* clock){
    memset(clock, 0, sizeof(rrClock));
}

void rr_clock_update(rrClock* clock, int time) {
    clock->update_time += time;
    clock->_flags = 0;

    while (clock->update_time >= 1000) {
        clock->update_time -= 1000;
        clock->seconds_pos += 1;
        clock->_flags |= RR_CLOCK_TICK_SECONDS;
    }

    while (clock->seconds_pos >= 30) {
        clock->seconds_pos -= 30;
        clock->minutes_pos += 1;
        clock->_flags |= RR_CLOCK_TICK_MINUTES;

        if (clock->minutes_pos >= 30) {
            clock->minutes_pos = 0;
        }

        if (clock->minutes_pos == clock->target_pos)
            clock->_flags |= RR_CLOCK_TICK_TARGET;
    }
}

void rr_clock_advance_target(rrClock* clock, int minutes) {
    clock->target_pos += minutes;

    if (clock->target_pos >= 30)
        clock->target_pos -= 30;
}

int rr_clock_did_tick_seconds(rrClock* clock) {
    return (clock->_flags & RR_CLOCK_TICK_SECONDS) == RR_CLOCK_TICK_SECONDS;
}

int rr_clock_did_tick_target(rrClock* clock) {
    return (clock->_flags & RR_CLOCK_TICK_TARGET) == RR_CLOCK_TICK_TARGET;
}
