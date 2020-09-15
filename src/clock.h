#ifndef RR_CLOCK_H
#define RR_CLOCK_H

/**
 * This structure represents the in game clock.
 * The clock has 30 discrete positions
 * Certain events are tied to clock ticks: time based score increases, and additional enemy spawns
 */
typedef struct {
    int update_time;
    int seconds_pos;
    int minutes_pos;
    int target_pos;
    unsigned int _flags;
} rrClock;

void rr_clock_init(rrClock* clock);

void rr_clock_update(rrClock* clock, int time);
void rr_clock_reset(rrClock* clock);

/** Returns a non-zero value if the clock ticked to a new 'seconds' value during the last call to update. */
int rr_clock_did_tick_seconds(rrClock* clock);

/** Returns a non-zero value if the clock ticked to a new 'minutes' value during the last call to update. */
int rr_clock_did_tick_minutes(rrClock* clock);
#endif
