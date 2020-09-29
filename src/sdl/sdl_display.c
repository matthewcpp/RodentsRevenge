#include "sdl_display.h"
#include "sdl_renderer.h"

#include "../enemy.h"
#include "../yarn.h"
#include "../ui/game_ui.h"
#include "../ui/title_ui.h"
#include "../util.h"

#include <SDL_image.h>

#include <assert.h>
#include <stdio.h>

#define RR_RENDERER_TILE_SIZE 16

typedef enum {
    RR_SCREEN_TITLE,
    RR_SCREEN_GAME
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


    rr_game_ui_destroy(display->game_ui);
    rr_title_ui_destroy(display->title_ui);

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

    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_BLOCK, 1, 1);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_MOUSE, 37, 37);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_WALL, 37, 55);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_CAT, 19, 1);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_CAT_WAIT, 37, 1);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_CHEESE, 55, 1);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_REMAINING_LIFE, 55, 37);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_HOLE, 19, 37);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_STUCK_PLAYER, 19, 55);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_MOUSETRAP, 1, 55);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_PLAYER_DEATH1, 1, 19);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_PLAYER_DEATH2, 19, 19);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_PLAYER_DEATH3, 37, 19);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_PLAYER_DEATH4, 55, 19);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_PLAYER_DEATH5, 1, 37);

    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_YARN, 55, 55);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_YARN_EXPLODE1, 73, 1);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_YARN_EXPLODE2, 73, 15);
    rr_sdl_display_add_tile_sprite_info(display, RR_SPRITESHEET_INDEX_YARN_EXPLODE3, 73, 33);
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

    sprintf(asset_path, "%s%s%s", asset_dir, rr_path_sep(), "clock.png");
    sprite = rr_renderer_load_sprite(display->renderer, asset_path);

    if (sprite)
        display->renderer->sprites[RR_SPRITE_CLOCK] = sprite;
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

void rr_sdl_display_draw_yarn(rrSDLDisplay* display, rrYarn* player) {
    rrSprite* sprite = display->spritesheet.sprites[RR_SPRITESHEET_INDEX_YARN];
    rrPoint draw_pos;
    rr_point_copy(&draw_pos, &display->_map_pos);
    draw_pos.x += player->entity.position.x * RR_RENDERER_TILE_SIZE;
    draw_pos.y += player->entity.position.y * RR_RENDERER_TILE_SIZE;

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
    }
}

void rr_sdl_display_draw_game_scren(rrSDLDisplay* display) {
    SDL_SetRenderDrawColor(display->sdl_renderer, 195, 195, 195, 255);
    SDL_RenderClear(display->sdl_renderer);

    rr_sdl_display_draw_board_background(display);
    rr_sdl_display_draw_entities(display);

    rr_game_ui_update(display->game_ui);
    rr_game_ui_draw(display->game_ui);

    SDL_RenderPresent(display->sdl_renderer);
}

void rr_sdl_display_draw_title_screen(rrSDLDisplay* display){
    SDL_SetRenderDrawColor(display->sdl_renderer, 195, 195, 195, 255);
    SDL_RenderClear(display->sdl_renderer);

    rr_title_ui_update(display->title_ui);
    rr_title_ui_draw(display->title_ui);

    SDL_RenderPresent(display->sdl_renderer);
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

void rr_sdl_display_on_game_exit(void* user_data) {
    rrSDLDisplay* display = (rrSDLDisplay*)user_data;
    rr_game_reset(display->_game);
    rr_game_set_active_level(display->_game, 1);
    rr_sdl_display_set_screen(display, RR_SCREEN_TITLE);
}

void rr_sdl_display_init_ui(rrSDLDisplay* display) {
    display->title_ui = rr_title_ui_create(display->renderer, display->_game, display->input);
    rr_ui_button_set_callback(&display->title_ui->new_game_button, rr_sdl_display_on_new_game, display);
    rr_ui_button_set_callback(&display->title_ui->level_select_dialog->ok_button, rr_sdl_display_on_select_level, display);

    display->game_ui = rr_game_ui_create(display->_game, display->renderer, display->input, &display->spritesheet);
    display->_map_pos.y =  display->game_ui->clock._sprite->rect.h + 20;
    rr_ui_button_set_callback(&display->game_ui->pause_dialog->exit_button, rr_sdl_display_on_game_exit, display);
}

void rr_sdl_display_draw(rrSDLDisplay* display) {
    switch (display->display_screen) {
        case RR_SCREEN_GAME:
            rr_sdl_display_draw_game_scren(display);
            break;
        case RR_SCREEN_TITLE:
            rr_sdl_display_draw_title_screen(display);
            break;
    }
}
