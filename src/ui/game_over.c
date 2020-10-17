#include "game_over.h"

#include "../assets.h"

void rr_ui_game_over_layout(rrUiGameOver* game_over);

void rr_ui_game_over_init(rrUiGameOver* game_over, rrRenderer* renderer){
    rrSprite* sprite;
    rrPoint point = {5,5};
    rrRect rect = {0,0,0,0};
    rrColor color;

    game_over->_renderer = renderer;

    sprite = rr_renderer_create_text(renderer, RR_FONT_TITLE, "Game Over");
    rr_color_black(&color);
    rr_renderer_set_sprite_tint_color(renderer, sprite, &color);
    rr_ui_basic_sprite_init(&game_over->text, &point, sprite, renderer);

    rect.w = sprite->rect.w;
    rect.h = sprite->rect.h;
    point.y += sprite->rect.h + 5;

    sprite = rr_renderer_create_text(renderer, RR_FONT_KEYBOARD, "Press Any Button");
    rr_renderer_set_sprite_tint_color(renderer, sprite, &color);
    rr_ui_basic_sprite_init(&game_over->caption, &point, sprite, renderer);

    rect.h += game_over->caption.sprite->rect.h + 5;

    /* add rect padding */
    rect.w += 10;
    rect.h += 10;

    rr_color_white(&color);
    rr_ui_basic_rect_init(&game_over->background, &rect, &color, renderer);

    rr_ui_game_over_layout(game_over);
}

void rr_ui_game_over_layout(rrUiGameOver* game_over) {
    rrPoint offset;
    rrPoint screen_size;
    rr_renderer_get_screen_size(game_over->_renderer, &screen_size);

    rr_point_set(&offset, screen_size.x / 2 - game_over->background.rect.w / 2, screen_size.y / 2 - game_over->background.rect.h / 2);

    game_over->background.rect.x += offset.x;
    game_over->background.rect.y += offset.y;
    rr_point_add_and_assign(&game_over->text.element.position, &offset);

    game_over->caption.element.position.y += offset.y;
    game_over->caption.element.position.x = screen_size.x / 2 - game_over->caption.sprite->rect.w / 2;
}

void rr_ui_game_over_draw(rrUiGameOver* game_over) {
    rr_ui_basic_rect_draw(&game_over->background);

    rr_ui_basic_sprite_draw(&game_over->text);
    game_over->text.element.position.x += 1;
    rr_ui_basic_sprite_draw(&game_over->text);
    game_over->text.element.position.x -= 1;

    rr_ui_basic_sprite_draw(&game_over->caption);
}
