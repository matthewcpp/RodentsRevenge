#include "sdl_display.h"
#include "sdl_renderer.h"

#include "../enemy.h"
#include "../yarn.h"
#include "../ui/game_ui.h"
#include "../ui/title_ui.h"
#include "../ui/high_scores_ui.h"
#include "../util.h"

#include <SDL_image.h>

#include <assert.h>
#include <stdio.h>

#define RR_RENDERER_TILE_SIZE 16

typedef enum {
    RR_SCREEN_TITLE,
    RR_SCREEN_GAME,
    RR_SCREEN_HIGH_SCORES
}rrDisplayScreen;

struct rrSDLDisplay{
    rrGame* _game;
    rrHighScores* _high_scores;

    rrPoint _map_pos;
    rrPoint window_size;
    rrDisplayScreen display_screen;

    rrRenderer* renderer;
    rrGameUi* game_ui;
    rrTitleUi* title_ui;
    rrHighScoresUi* high_scores_ui;
    rrInput* input;
};

rrSDLDisplay* rr_sdl_display_create(rrGame* game, rrHighScores* high_scores, rrInput* input, rrRenderer* sdl_renderer) {
    rrSDLDisplay* display = malloc(sizeof(rrSDLDisplay));

    display->_game = game;
    display->_high_scores = high_scores;

    display->input = input;
    display->renderer = sdl_renderer;
    display->display_screen = RR_SCREEN_TITLE;

    display->game_ui = NULL;
    display->title_ui = NULL;

    rr_renderer_get_screen_size(display->renderer, &display->window_size);
    display->_map_pos.x = (display->window_size.x / 2)  - ((RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE) / 2);
    display->_map_pos.y = (display->window_size.y / 2)  - ((RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE) / 2);

    return display;
}

void rr_sdl_display_destroy(rrSDLDisplay* display) {
    rr_game_ui_destroy(display->game_ui);
    rr_title_ui_destroy(display->title_ui);
    rr_high_scores_ui_destroy(display->high_scores_ui);

    free(display);
}

void rr_sdl_display_draw_board_background(rrSDLDisplay* display) {
    rrColor background_color = {195, 195, 0, 255};
    rrRect board_rect;
    board_rect.x = display->_map_pos.x;
    board_rect.y = display->_map_pos.y;
    board_rect.w = RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE;
    board_rect.h = RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE;

    rr_renderer_color(display->renderer, &background_color);
    rr_renderer_fill_rect(display->renderer, &board_rect);
}

void rr_sdl_display_draw_enemy(rrSDLDisplay* display, rrEnemy* enemy) {
    rrSprite* sprite;
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &display->_map_pos);
    draw_pos.x += enemy->entity.position.x * RR_RENDERER_TILE_SIZE;
    draw_pos.y += enemy->entity.position.y * RR_RENDERER_TILE_SIZE;

    switch (enemy->entity.status) {
        case RR_STATUS_ACTIVE:
            sprite = display->renderer->spritesheet->sprites[RR_SPRITESHEET_INDEX_CAT];
            break;

        case RR_STATUS_WAITING:
            sprite = display->renderer->spritesheet->sprites[RR_SPRITESHEET_INDEX_CAT_WAIT];
            break;

        default:
            return;
    }

    rr_renderer_draw_sprite(display->renderer, sprite, &draw_pos);
}

void rr_sdl_display_draw_player(rrSDLDisplay* display, rrPlayer* player) {
    rrSprite* sprite;
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &display->_map_pos);
    draw_pos.x += player->entity.position.x * RR_RENDERER_TILE_SIZE;
    draw_pos.y += player->entity.position.y * RR_RENDERER_TILE_SIZE;

    switch (player->entity.status) {
        case RR_STATUS_ACTIVE:
            sprite = display->renderer->spritesheet->sprites[RR_SPRITESHEET_INDEX_MOUSE];
            break;

        case RR_STATUS_STUCK:
            sprite = display->renderer->spritesheet->sprites[RR_SPRITESHEET_INDEX_STUCK_PLAYER];
            break;

        case RR_STATUS_DYING:
            sprite = rr_animation_player_get_current_sprite(&player->death_animation);
            break;

        default:
            return;
    }

    rr_renderer_draw_sprite(display->renderer, sprite, &draw_pos);
}

void rr_sdl_display_draw_yarn(rrSDLDisplay* display, rrYarn* yarn) {
    rrSprite* sprite;
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &display->_map_pos);
    draw_pos.x += yarn->entity.position.x * RR_RENDERER_TILE_SIZE;
    draw_pos.y += yarn->entity.position.y * RR_RENDERER_TILE_SIZE;

    if (yarn->entity.status == RR_STATUS_DYING)
        sprite = rr_animation_player_get_current_sprite(&yarn->explode_animation);
    else
        sprite = display->renderer->spritesheet->sprites[RR_SPRITESHEET_INDEX_YARN];

    rr_renderer_draw_sprite(display->renderer, sprite, &draw_pos);
}

void rr_sdl_display_draw_basic_block(rrSDLDisplay* display, rrSpriteSheetIndex sprite, rrPoint* cell) {
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &display->_map_pos);
    draw_pos.x += cell->x * RR_RENDERER_TILE_SIZE;
    draw_pos.y += cell->y * RR_RENDERER_TILE_SIZE;

    rr_renderer_draw_sprite(display->renderer, display->renderer->spritesheet->sprites[sprite], &draw_pos);
}

void rr_sdl_display_draw_entities(rrSDLDisplay* display) {
    rrPoint cell;
    for (cell.x = 0; cell.x < RR_GRID_WIDTH; cell.x++) {
        for (cell.y = 0; cell.y < RR_GRID_HEIGHT; cell.y++) {
            rrEntity* entity = rr_grid_get_entity_at_position(rr_game_get_grid(display->_game), &cell);

            if (!entity)
                continue;

            switch (entity->type) {
                case RR_ENTITY_WALL:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITESHEET_INDEX_WALL, &cell);
                    break;

                case RR_ENTITY_BLOCK:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITESHEET_INDEX_BLOCK, &cell);
                    break;

                case RR_ENTITY_CHEESE:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITESHEET_INDEX_CHEESE, &cell);
                    break;

                case RR_ENTITY_HOLE:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITESHEET_INDEX_HOLE, &cell);
                    break;

                case RR_ENTITY_TRAP:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITESHEET_INDEX_MOUSETRAP, &cell);
                    break;

                case RR_ENTITY_ENEMY:
                    rr_sdl_display_draw_enemy(display, (rrEnemy*)entity);
                    break;

                case RR_ENTITY_YARN:
                    rr_sdl_display_draw_yarn(display, (rrYarn*)entity);
                    break;

                case RR_ENTITY_PLAYER:
                    rr_sdl_display_draw_player(display, (rrPlayer*)entity);
                    break;

                case RR_ENTITY_UNKNOWN:
                    break;
            }
        }
    }
}

void rr_sdl_display_set_screen(rrSDLDisplay* display, rrDisplayScreen screen) {
    display->display_screen = screen;

    switch (display->display_screen) {
        case RR_SCREEN_TITLE:
            rr_title_ui_show(display->title_ui);
            break;

        case RR_SCREEN_GAME:
            rr_game_ui_show(display->game_ui);
            break;

        case RR_SCREEN_HIGH_SCORES:
            rr_high_scores_ui_show(display->high_scores_ui);
            break;
    }
}

void rr_sdl_display_on_new_game(void* user_data) {
    rrSDLDisplay* display = (rrSDLDisplay*)user_data;
    rr_sdl_display_set_screen(display, RR_SCREEN_GAME);
    rr_game_restart(display->_game);
}

void rr_sdl_display_on_select_level(void* user_data) {
    rrSDLDisplay* display = (rrSDLDisplay*)user_data;

    rr_sdl_display_set_screen(display, RR_SCREEN_GAME);
    rr_game_set_active_level(display->_game, display->title_ui->level_select_dialog->current_level);
    rr_game_restart(display->_game);
}

void rr_sdl_display_on_high_scores(void* user_data) {
    rrSDLDisplay* display = (rrSDLDisplay*)user_data;
    rr_sdl_display_set_screen(display, RR_SCREEN_HIGH_SCORES);
}

void rr_sdl_display_on_game_exit(void* user_data) {
    rrSDLDisplay* display = (rrSDLDisplay*)user_data;
    rr_game_reset(display->_game);
    rr_game_set_active_level(display->_game, 1);
    rr_sdl_display_set_screen(display, RR_SCREEN_TITLE);
}

void rr_sdl_on_high_scores_back_button(void* user_data) {
    rrSDLDisplay* display = (rrSDLDisplay*)user_data;
    rr_sdl_display_set_screen(display, RR_SCREEN_TITLE);
}

void rr_sdl_display_init_ui(rrSDLDisplay* display) {
    display->title_ui = rr_title_ui_create(display->renderer, display->_game, display->input);
    rr_ui_button_set_callback(&display->title_ui->new_game_button, rr_sdl_display_on_new_game, display);
    rr_ui_button_set_callback(&display->title_ui->level_select_dialog->ok_button, rr_sdl_display_on_select_level, display);
    rr_ui_button_set_callback(&display->title_ui->high_scores_button, rr_sdl_display_on_high_scores, display);

    display->game_ui = rr_game_ui_create(display->_game, display->_high_scores, display->renderer, display->input, display->renderer->spritesheet);
    display->_map_pos.y =  display->game_ui->clock._sprite->rect.h + 20;
    rr_ui_button_set_callback(&display->game_ui->pause_dialog->exit_button, rr_sdl_display_on_game_exit, display);

    display->high_scores_ui = rr_high_scores_ui_create(display->renderer, display->_high_scores, display->input);
    rr_ui_button_set_callback(&display->high_scores_ui->back_button, rr_sdl_on_high_scores_back_button, display);
}

void rr_sdl_display_update(rrSDLDisplay* display) {
    switch (display->display_screen) {
        case RR_SCREEN_GAME: {
            rr_game_ui_update(display->game_ui);

            if (display->game_ui->_previous_state == RR_GAME_STATE_OVER && rr_input_button_down(display->input, RR_INPUT_BUTTON_START))
                rr_sdl_display_set_screen(display, RR_SCREEN_TITLE);

            break;
        }
        case RR_SCREEN_TITLE:
            rr_title_ui_update(display->title_ui);
            break;

        case RR_SCREEN_HIGH_SCORES:
            rr_high_scores_ui_update(display->high_scores_ui);
            break;
    }
}

void rr_sdl_display_draw(rrSDLDisplay* display) {
    switch (display->display_screen) {
        case RR_SCREEN_GAME:
            rr_sdl_display_draw_board_background(display);
            rr_sdl_display_draw_entities(display);
            rr_game_ui_draw(display->game_ui);
            break;
        case RR_SCREEN_TITLE:
            rr_title_ui_draw(display->title_ui);
            break;

        case RR_SCREEN_HIGH_SCORES:
            rr_high_scores_ui_draw(display->high_scores_ui);
            break;
    }
}
