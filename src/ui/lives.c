#include "lives.h"

#define LIVES_SPRITE_SPACER 5;

void rr_ui_lives_init(rrUILives* lives, rrSprite* sprite, rrPoint* top_left_offset, rrGame* game, rrRenderer* renderer) {
    lives->sprite = sprite;
    lives->_game = game;
    lives->_renderer = renderer;

    rr_point_copy(&lives->offset, top_left_offset);
}

void rr_ui_lives_draw(rrUILives* lives) {
    int i;
    rrPlayer* player = rr_game_get_player(lives->_game);
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &lives->offset);

    for (i = 0; i < player->lives_remaining; i++) {
        rr_renderer_draw_sprite(lives->_renderer, lives->sprite, &draw_pos);
        draw_pos.x += lives->sprite->rect.w + LIVES_SPRITE_SPACER;
    }
}
