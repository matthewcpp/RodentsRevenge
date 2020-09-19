#include "clock.h"

#include "../vec2.h"

#include <assert.h>
#include <math.h>

#define RR_CLOCK_HAND_LENGTH 9.0
#define RR_CLOCK_TARGET_LENGTH 10.0

#define RR_PI 3.14159265358979323846264338327950288

void rr_ui_clock_init(rrUiClock* clock, rrRenderer* renderer, rrGame* game, rrSprite* sprite, rrPoint* offset) {
    rrPoint screen_size;
    assert(sprite != NULL);

    clock->_renderer = renderer;
    clock->_game = game;
    clock->_sprite = sprite;
    rr_point_copy(&clock->offset, offset);

    rr_renderer_get_screen_size(clock->_renderer, &screen_size);
    clock->pos.x = screen_size.x / 2 - clock->_sprite->rect.w / 2 + clock->offset.x;
    clock->pos.y = clock->offset.y;
}

void rr_sdl_display_draw_clock_hand(rrUiClock* clock, rrPoint* clock_center, int pos) {
    rrPoint clock_hand;
    double radians = (12.0 * pos - 90.0) * RR_PI / 180.0;

    rr_point_copy(&clock_hand, clock_center);

    clock_hand.x += (int)(cos(radians) * RR_CLOCK_HAND_LENGTH);
    clock_hand.y += (int)(sin(radians) * RR_CLOCK_HAND_LENGTH);

    rr_renderer_draw_line(clock->_renderer, clock_center, &clock_hand);
}

void rr_sdl_display_draw_clock_target(rrUiClock* clock, rrPoint* clock_center_pt, int target_pos) {
    rrVec2 center_vec, target_end_pos, center_dir;
    double radians = (12.0 * target_pos - 90.0) * RR_PI / 180.0;
    rrPoint target_start_pt, target_end_pt;

    rr_vec2_from_point(&center_vec, clock_center_pt);
    rr_vec2_copy(&target_end_pos, &center_vec);

    target_end_pos.x += (float)(cos(radians) * RR_CLOCK_TARGET_LENGTH);
    target_end_pos.y += (float)(sin(radians) * RR_CLOCK_TARGET_LENGTH);

    rr_vec2_sub(&center_dir, &center_vec, &target_end_pos);
    rr_vec2_normalize(&center_dir);
    rr_vec2_scale(&center_dir, &center_dir, RR_CLOCK_TARGET_LENGTH / 2.0f);
    rr_vec2_add(&center_vec, &target_end_pos, &center_dir);

    rr_vec2_to_point(&target_start_pt, &center_vec);
    rr_vec2_to_point(&target_end_pt, &target_end_pos);

    rr_renderer_draw_line(clock->_renderer, &target_start_pt, &target_end_pt);
}

static rrPoint sprite_center_offset = {14, 17};

void rr_ui_clock_draw(rrUiClock* clock) {
    rrClock* game_clock = rr_game_get_clock(clock->_game);
    rrPoint clock_center;
    rrColor color;

    rr_renderer_draw_sprite(clock->_renderer, clock->_sprite, &clock->pos);

    rr_point_add(&clock_center, &clock->pos, &sprite_center_offset);

    /* draw second hand */
    rr_color_red(&color);
    rr_renderer_color(clock->_renderer, &color);
    rr_sdl_display_draw_clock_hand(clock, &clock_center, game_clock->seconds_pos);

    /* draw minute hand */
    rr_color_blue(&color);
    rr_renderer_color(clock->_renderer, &color);
    rr_sdl_display_draw_clock_hand(clock, &clock_center, game_clock->minutes_pos);

    /* draw target mark */
    rr_color_blue(&color);
    rr_renderer_color(clock->_renderer, &color);
    rr_sdl_display_draw_clock_target(clock, &clock_center, game_clock->target_pos);
}
