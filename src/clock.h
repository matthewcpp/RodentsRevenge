#ifndef RR_CLOCK_H
#define RR_CLOCK_H

typedef struct rrClock rrClock;

rrClock* rr_clock_create();
void rr_clock_destroy(rrClock* clock);

void rr_clock_update(rrClock* clock, int time);
void rr_clock_get(rrClock* clock, int* seconds_pos, int* minutes_pos, int* target_pos);
void rr_clock_reset(rrClock* clock);
#endif
