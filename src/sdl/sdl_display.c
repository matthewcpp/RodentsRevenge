#include "sdl_display.h"
#include "../draw/animation.h"
#include "sdl_renderer.h"

#include "../enemy.h"
#include "../vec2.h"
#include "../ui/game_ui.h"
#include "../ui/title_ui.h"
#include "../ui/level_select_ui.h"
#include "../draw/spritesheet.h"
#include "../assets.h"
#include "../util.h"

#include <SDL_image.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#define RR_RENDERER_TILE_SIZE 16

typedef enum {
    RR_SCREEN_TITLE,
    RR_SCREEN_GAME,
    RR_SCREEN_LEVEL_SELECT
}rrDisplayScreen;

struct rrSDLDisplay{
    rrGame* _game;
    SDL_Window* _window;
    SDL_Renderer* sdl_renderer;
    rrSpritesheet spritesheet;

    rrPoint _map_pos;
    rrPoint window_size;
    rrDisplayScreen display_screen;

    rrRenderer* renderer;
    rrGameUi* game_ui;
    rrTitleUi* title_ui;
    rrUILevelSelect * level_select_ui;
    rrInput* input;
};

rrSDLDisplay* rr_sdl_display_create(SDL_Window* window, rrGame* game, rrInput* input) {
    rrSDLDisplay* display = malloc(sizeof(rrSDLDisplay));

    display->_window = window;
    display->_game = game;
    display->input = input;
    display->sdl_renderer = SDL_CreateRenderer(window, -1, 0);
    display->renderer = rr_sdl_renderer_create(display->sdl_renderer);
    display->display_screen = RR_SCREEN_TITLE;

    display->game_ui = NULL;
    display->title_ui = NULL;
    display->level_select_ui = NULL;

    TTF_Init();

    SDL_GetRendererOutputSize(display->sdl_renderer, &display->window_size.x, &display->window_size.y);
    display->_map_pos.x = (display->window_size.x / 2)  - ((RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE) / 2);
    display->_map_pos.y = (display->window_size.y / 2)  - ((RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE) / 2);

    rr_sdl_renderer_set_screen_size(display->renderer, &display->window_size);

    return display;
}

void rr_sdl_display_destroy(rrSDLDisplay* display) {
    rr_animation_destroy(rr_game_get_player(display->_game)->death_animation);

    rr_sdl_renderer_destroy(display->renderer);
    SDL_DestroyRenderer(display->sdl_renderer);

    if (display->level_select_ui)
        rr_ui_level_select_delete(display->level_select_ui);

    TTF_Quit();

    free(display);
}

void rr_sdl_display_add_sprite_info(rrSDLDisplay* display, rrSpriteSheetIndex index, int x, int y, int w, int h) {
    rrRect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    rr_spritesheet_add_sprite(&display->spritesheet, index, &rect);
}

void rr_sdl_display_add_tile_sprite_info(rrSDLDisplay* display, rrSpriteSheetIndex index, int x, int y) {
    rr_sdl_display_add_sprite_info(display, index, x, y, 16, 16);
}

void rr_sdl_display_init_spritesheet(rrSDLDisplay* display) {
    rr_spritesheet_init(&display->spritesheet, display->renderer->sprites[RR_SPRITE_SPRITESHEET], RR_SPRITESHEET_INDEX_COUNT);

    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_BLOCK, 18, 0);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_MOUSE, 54, 34);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_WALL, 36, 0);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_CAT, 85, 52);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_CAT_WAIT, 0, 36);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_CHEESE, 36, 54);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_REMAINING_LIFE, 36, 18);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_HOLE, 0, 0);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_STUCK_PLAYER, 0, 54);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_MOUSETRAP, 18, 54);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_PLAYER_DEATH1, 18, 36);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_PLAYER_DEATH2, 54, 54);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_PLAYER_DEATH3, 36, 36);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_PLAYER_DEATH4, 0, 18);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_PLAYER_DEATH5, 18, 18);
    rr_sdl_display_add_sprite_info(display, RR_SPRITESHEET_INDEX_CLOCK, 54, 0, 29, 32);
}

void rr_sdl_display_init_animation(rrSDLDisplay* display) {
    rrSprite* animation_frames[5];

    animation_frames[0] = display->spritesheet.sprites[RR_SPRITESHEET_INDEX_PLAYER_DEATH1];
    animation_frames[1] = display->spritesheet.sprites[RR_SPRITESHEET_INDEX_PLAYER_DEATH2];
    animation_frames[2] = display->spritesheet.sprites[RR_SPRITESHEET_INDEX_PLAYER_DEATH3];
    animation_frames[3] = display->spritesheet.sprites[RR_SPRITESHEET_INDEX_PLAYER_DEATH4];
    animation_frames[4] = display->spritesheet.sprites[RR_SPRITESHEET_INDEX_PLAYER_DEATH5];

    rr_game_get_player(display->_game)->death_animation = rr_animation_create(&display->spritesheet, 5, animation_frames, 100);
}

int rr_sdl_display_load_sprites(rrSDLDisplay* display, const char* asset_dir) {
    rrSprite* sprite;
    char asset_path[256];
    sprintf(asset_path, "%s%s%s", asset_dir, rr_path_sep(), "spritesheet.png");

    sprite = rr_renderer_load_sprite(display->renderer, asset_path);
    if (sprite) {
        display->renderer->sprites[RR_SPRITE_SPRITESHEET] = sprite;
        rr_sdl_display_init_spritesheet(display);
        rr_sdl_display_init_animation(display);
    }
    else {
        return 0;
    }

    sprintf(asset_path, "%s%s%s", asset_dir, rr_path_sep(), "logo.png");
    sprite = rr_renderer_load_sprite(display->renderer, asset_path);

    if (sprite)
        display->renderer->sprites[RR_SPRITE_TITLE_LOGO] = sprite;
    else
        return 0;

    return 1;
}


int rr_sdl_display_load_fonts(rrSDLDisplay* display, const char* asset_dir) {
    TTF_Font* font;
    char asset_path[256];
    sprintf(asset_path, "%s%s%s", asset_dir, rr_path_sep(), "ms-sans-serif.ttf");

    font = TTF_OpenFont(asset_path, 24);
    if (font)
        display->renderer->fonts[RR_FONT_TITLE] = font;
    else
        return 0;

    font = TTF_OpenFont(asset_path, 16);
    if (font)
        display->renderer->fonts[RR_FONT_BUTTON] = font;
    else
        return 0;


    return 1;
}

void rr_sdl_display_draw_board_background(rrSDLDisplay* display) {
    SDL_Rect board_rect;
    board_rect.x = display->_map_pos.x;
    board_rect.y = display->_map_pos.y;
    board_rect.w = RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE;
    board_rect.h = RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE;

    SDL_SetRenderDrawColor(display->sdl_renderer, 195, 195, 0, 255);
    SDL_RenderFillRect(display->sdl_renderer, &board_rect);
}

void rr_sdl_display_draw_enemy(rrSDLDisplay* display, rrEnemy* enemy) {
    rrSpriteSheetIndex sprite_index;
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &display->_map_pos);
    draw_pos.x += enemy->entity.position.x * RR_RENDERER_TILE_SIZE;
    draw_pos.y += enemy->entity.position.y * RR_RENDERER_TILE_SIZE;

    switch (enemy->entity.status) {
        case RR_STATUS_ACTIVE:
            sprite_index = RR_SPRITESHEET_INDEX_CAT;
            break;

        case RR_STATUS_WAITING:
            sprite_index = RR_SPRITESHEET_INDEX_CAT_WAIT;
            break;

        default:
            return;
    }

    rr_renderer_draw_sprite(display->renderer, display->spritesheet.sprites[sprite_index], &draw_pos);
}

void rr_sdl_display_draw_player(rrSDLDisplay* display, rrPlayer* player) {
    rrSprite* sprite;
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &display->_map_pos);
    draw_pos.x += player->entity.position.x * RR_RENDERER_TILE_SIZE;
    draw_pos.y += player->entity.position.y * RR_RENDERER_TILE_SIZE;

    switch (player->entity.status) {
        case RR_STATUS_ACTIVE:
            sprite = display->spritesheet.sprites[RR_SPRITESHEET_INDEX_MOUSE];
            break;

        case RR_STATUS_STUCK:
            sprite = display->spritesheet.sprites[RR_SPRITESHEET_INDEX_STUCK_PLAYER];
            break;

        case RR_STATUS_DYING:
            sprite = rr_animation_get_current_sprite(player->death_animation);
            break;

        default:
            return;
    }

    rr_renderer_draw_sprite(display->renderer, sprite, &draw_pos);
}

void rr_sdl_display_draw_basic_block(rrSDLDisplay* display, rrSpriteSheetIndex sprite, rrPoint* cell) {
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &display->_map_pos);
    draw_pos.x += cell->x * RR_RENDERER_TILE_SIZE;
    draw_pos.y += cell->y * RR_RENDERER_TILE_SIZE;

    rr_renderer_draw_sprite(display->renderer, display->spritesheet.sprites[sprite], &draw_pos);
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

                case RR_ENTITY_PLAYER:
                    rr_sdl_display_draw_player(display, (rrPlayer*)entity);
                    break;

                case RR_ENTITY_UNKNOWN:
                    break;
            }
        }
    }
}

void rr_sdl_display_draw_game_scren(rrSDLDisplay* display) {
    if (display->game_ui == NULL){
        display->game_ui = rr_game_ui_create(display->_game, display->renderer, display->input, &display->spritesheet);
        display->_map_pos.y =  display->game_ui->clock._sprite->rect.h + 20;
    }


    SDL_SetRenderDrawColor(display->sdl_renderer, 195, 195, 195, 255);
    SDL_RenderClear(display->sdl_renderer);

    /* TODO: Figure out why this order is necessary */
    rr_sdl_display_draw_board_background(display);
    rr_sdl_display_draw_entities(display);

    rr_game_ui_update(display->game_ui);
    rr_game_ui_draw(display->game_ui);

    SDL_RenderPresent(display->sdl_renderer);
}

void rr_sdl_display_on_new_game(void* user_data) {
    rrSDLDisplay* display = (rrSDLDisplay*)user_data;
    display->display_screen = RR_SCREEN_GAME;
}

void rr_sdl_display_on_level_select(void* user_data) {
    rrSDLDisplay* display = (rrSDLDisplay*)user_data;
    display->display_screen = RR_SCREEN_LEVEL_SELECT;
}

void rr_sdl_display_draw_title_screen(rrSDLDisplay* display){
    SDL_SetRenderDrawColor(display->sdl_renderer, 195, 195, 195, 255);
    SDL_RenderClear(display->sdl_renderer);

    if (!display->title_ui) {
        display->title_ui = rr_title_ui_create(display->renderer, display->_game, display->input);
        rr_ui_button_set_callback(&display->title_ui->buttons[0], rr_sdl_display_on_new_game, display);
        rr_ui_button_set_callback(&display->title_ui->buttons[1], rr_sdl_display_on_level_select, display);
    }

    rr_title_ui_update(display->title_ui);
    rr_title_ui_draw(display->title_ui);

    SDL_RenderPresent(display->sdl_renderer);
}

void rr_sdl_display_on_level_select_screen_cancel(void* user_data) {
    rrSDLDisplay* display = (rrSDLDisplay*)user_data;
    display->display_screen = RR_SCREEN_TITLE;
}

void rr_sdl_display_on_level_select_screen_ok(void* user_data) {
    rrSDLDisplay* display = (rrSDLDisplay*)user_data;
    rr_game_set_active_level(display->_game, display->level_select_ui->current_level);
    display->display_screen = RR_SCREEN_GAME;
}

void rr_sdl_display_draw_level_select_screen(rrSDLDisplay* display){
    SDL_SetRenderDrawColor(display->sdl_renderer, 195, 195, 195, 255);
    SDL_RenderClear(display->sdl_renderer);

    if (!display->level_select_ui) {
        display->level_select_ui = rr_ui_level_select_create(display->_game, display->renderer, display->input);
        rr_ui_button_set_callback(&display->level_select_ui->cancel_button, rr_sdl_display_on_level_select_screen_cancel, display);
        rr_ui_button_set_callback(&display->level_select_ui->ok_button, rr_sdl_display_on_level_select_screen_ok, display);
    }

    rr_title_ui_draw(display->title_ui);
    rr_ui_level_select_update(display->level_select_ui);
    rr_ui_level_select_draw(display->level_select_ui);

    SDL_RenderPresent(display->sdl_renderer);
}

void rr_sdl_display_draw(rrSDLDisplay* display) {
    switch (display->display_screen) {
        case RR_SCREEN_GAME:
            rr_sdl_display_draw_game_scren(display);
            break;
        case RR_SCREEN_TITLE:
            rr_sdl_display_draw_title_screen(display);
            break;
        case RR_SCREEN_LEVEL_SELECT:
            rr_sdl_display_draw_level_select_screen(display);
            break;
    }
}
