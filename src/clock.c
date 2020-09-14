#include "clock.h"

#include <stdlib.h>
#include <string.h>

struct rrClock {
    int update_time;
    int seconds_pos;
    int minutes_pos;
    int target_pos;
};

rrClock* rr_clock_create() {
    return calloc(1, sizeof(rrClock));
}

void rr_clock_reset(rrClock* clock){
    memset(clock, 0, sizeof(rrClock));
}

void rr_clock_destroy(rrClock* clock) {
    free(clock);
}

void rr_clock_update(rrClock* clock, int time) {
    clock->update_time += time;

    if (clock->update_time > 1000) {
        clock->update_time -= 1000;
        clock->seconds_pos += 1;
    }

    if (clock->seconds_pos > 29) {
        clock->seconds_pos = 0;
        clock->minutes_pos += 1;
    }

    if (clock->minutes_pos > 29) {
        clock->minutes_pos = 0;
    }
}

void rr_clock_get(rrClock* clock, int* seconds_pos, int* minutes_pos, int* target_pos) {
    *seconds_pos = clock->seconds_pos;
    *minutes_pos = clock->minutes_pos;
    *target_pos = clock->target_pos;
}
