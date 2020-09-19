#include "sdl_display.h"
#include "../draw/animation.h"
#include "sdl_renderer.h"

#include "../enemy.h"
#include "../vec2.h"
#include "../ui/ui.h"
#include "../draw/spritesheet.h"
#include "../draw/sprites.h"

#include <SDL_image.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#define RR_RENDERER_TILE_SIZE 16

struct rrSDLDisplay{
    rrGame* _game;
    SDL_Window* _window;
    SDL_Renderer* sdl_renderer;
    rrSpritesheet spritesheet;
    TTF_Font* _font;

    rrPoint _map_pos;
    rrPoint window_size;

    rrRenderer* renderer;
    rrUi* ui;
    rrInput* input;
};

rrSDLDisplay* rr_sdl_display_create(SDL_Window* window, rrGame* game, rrInput* input) {
    rrSDLDisplay* display = malloc(sizeof(rrSDLDisplay));

    display->_window = window;
    display->_game = game;
    display->input = input;
    display->sdl_renderer = SDL_CreateRenderer(window, -1, 0);
    display->renderer = rr_sdl_renderer_create(display->sdl_renderer);

    display->_font = NULL;
    display->ui = NULL;

    TTF_Init();

    SDL_GetRendererOutputSize(display->sdl_renderer, &display->window_size.x, &display->window_size.y);
    display->_map_pos.x = (display->window_size.x / 2)  - ((RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE) / 2);
    display->_map_pos.y = (display->window_size.y / 2)  - ((RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE) / 2);

    rr_sdl_renderer_set_screen_size(display->renderer, &display->window_size);

    return display;
}

void rr_sdl_display_destroy(rrSDLDisplay* display) {
    rr_animation_destroy(rr_game_get_player(display->_game)->death_animation);

    SDL_DestroyRenderer(display->sdl_renderer);

    rr_ui_destroy(display->ui);
    rr_sdl_renderer_destroy(display->renderer);

    if (display->_font)
        TTF_CloseFont(display->_font);

    TTF_Quit();

    free(display);
}

void rr_sdl_display_add_sprite_info(rrSDLDisplay* display, SpriteIndex index, int x, int y, int w, int h) {
    rrRect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    rr_spritesheet_add_sprite(&display->spritesheet, index, &rect);
}

void rr_sdl_display_add_tile_sprite_info(rrSDLDisplay* display, SpriteIndex index, int x, int y) {
    rr_sdl_display_add_sprite_info(display, index, x, y, 16, 16);
}

int rr_sdl_display_load_spritesheet(rrSDLDisplay* display, const char* path) {
    rrSprite* frames[5];
    rrSprite* sprite = rr_renderer_load_sprite(display->renderer, path);
    if (!sprite)
        return 0;

    rr_spritesheet_init(&display->spritesheet, sprite, RR_SPRITE_COUNT);

    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_BLOCK, 18, 0);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_MOUSE, 54, 34);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_WALL, 36, 0);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_CAT, 85, 52);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_CAT_WAIT, 0, 36);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_CHEESE, 36, 54);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_REMAINING_LIFE, 36, 18);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_HOLE, 0, 0);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_STUCK_PLAYER, 0, 54);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_MOUSETRAP, 18, 54);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_PLAYER_DEATH1, 18, 36);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_PLAYER_DEATH2, 54, 54);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_PLAYER_DEATH3, 36, 36);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_PLAYER_DEATH4, 0, 18);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITE_PLAYER_DEATH5, 18, 18);
    rr_sdl_display_add_sprite_info(display, RR_SPRITE_CLOCK, 54, 0, 29, 32);

    frames[0] = display->spritesheet.sprites[RR_SPRITE_PLAYER_DEATH1];
    frames[1] = display->spritesheet.sprites[RR_SPRITE_PLAYER_DEATH2];
    frames[2] = display->spritesheet.sprites[RR_SPRITE_PLAYER_DEATH3];
    frames[3] = display->spritesheet.sprites[RR_SPRITE_PLAYER_DEATH4];
    frames[4] = display->spritesheet.sprites[RR_SPRITE_PLAYER_DEATH5];

    rr_game_get_player(display->_game)->death_animation = rr_animation_create(&display->spritesheet, 5, frames, 100);

    return 1;
}

int rr_sdl_display_load_font(rrSDLDisplay* display, const char* path) {
    if (display->_font)
        TTF_CloseFont(display->_font);

    display->_font = TTF_OpenFont(path, 18);
    rr_sdl_renderer_set_font(display->renderer, display->_font);

    return display->_font != NULL;
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
    SpriteIndex sprite_index;
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &display->_map_pos);
    draw_pos.x += enemy->entity.position.x * RR_RENDERER_TILE_SIZE;
    draw_pos.y += enemy->entity.position.y * RR_RENDERER_TILE_SIZE;

    switch (enemy->entity.status) {
        case RR_STATUS_ACTIVE:
            sprite_index = RR_SPRITE_CAT;
            break;

        case RR_STATUS_WAITING:
            sprite_index = RR_SPRITE_CAT_WAIT;
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
            sprite = display->spritesheet.sprites[RR_SPRITE_MOUSE];
            break;

        case RR_STATUS_STUCK:
            sprite = display->spritesheet.sprites[RR_SPRITE_STUCK_PLAYER];
            break;

        case RR_STATUS_DYING:
            sprite = rr_animation_get_current_sprite(player->death_animation);
            break;

        default:
            return;
    }

    rr_renderer_draw_sprite(display->renderer, sprite, &draw_pos);
}

void rr_sdl_display_draw_basic_block(rrSDLDisplay* display, SpriteIndex sprite, rrPoint* cell) {
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
                    rr_sdl_display_draw_basic_block(display, RR_SPRITE_WALL, &cell);
                    break;

                case RR_ENTITY_BLOCK:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITE_BLOCK, &cell);
                    break;

                case RR_ENTITY_CHEESE:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITE_CHEESE, &cell);
                    break;

                case RR_ENTITY_HOLE:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITE_HOLE, &cell);
                    break;

                case RR_ENTITY_TRAP:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITE_MOUSETRAP, &cell);
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

void rr_sdl_display_draw(rrSDLDisplay* display) {

    if (display->ui == NULL){
        display->ui = rr_ui_create(display->_game, display->renderer, display->input, &display->spritesheet);
        display->_map_pos.y = display->ui->menu.bar_height + display->ui->clock._sprite->rect.h + 20;
    }


    SDL_SetRenderDrawColor(display->sdl_renderer, 195, 195, 195, 255);
    SDL_RenderClear(display->sdl_renderer);

    /* TODO: Figure out why this order is necessary */
    rr_sdl_display_draw_board_background(display);
    rr_sdl_display_draw_entities(display);

    rr_ui_update(display->ui);
    rr_ui_draw(display->ui);

    SDL_RenderPresent(display->sdl_renderer);
}
