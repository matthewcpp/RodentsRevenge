#include "sdl_display.h"

#include <SDL_image.h>

#include <stdio.h>
#include <assert.h>

#define RR_RENDERER_TILE_SIZE 16

void rr_sdl_display_init(SDL_Window* window, rrSDLDisplay* renderer, rrGame* game) {
    renderer->_window = window;
    renderer->_renderer = SDL_CreateRenderer(window, -1, 0);
    renderer->_game = game;
    renderer->_spritesheet = NULL;
    renderer->_font = NULL;
    renderer->_scoreText = NULL;

    TTF_Init();
}

void rr_sdl_display_uninit(rrSDLDisplay* renderer) {
    SDL_DestroyTexture(renderer->_spritesheet);
    SDL_DestroyRenderer(renderer->_renderer);

    if (renderer->_font)
        TTF_CloseFont(renderer->_font);

    TTF_Quit();
}

void rr_sdl_display_sprite_info(SDL_Rect* rect, int x, int y) {
    rect->x = x;
    rect->y = y;
    rect->w = 16;
    rect->h = 16;
}

int rr_sdl_display_load_spritesheet(rrSDLDisplay* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);

    if (!surface)
        return 0;

    renderer->_spritesheet = SDL_CreateTextureFromSurface(renderer->_renderer, surface);
    SDL_FreeSurface(surface);

    rr_sdl_display_sprite_info(renderer->_sprites + RR_SPRITE_BLOCK, 1, 1);
    rr_sdl_display_sprite_info(renderer->_sprites + RR_SPRITE_MOUSE, 37, 19);
    rr_sdl_display_sprite_info(renderer->_sprites + RR_SPRITE_WALL, 37, 37);
    rr_sdl_display_sprite_info(renderer->_sprites + RR_SPRITE_CAT, 19, 1);

    return 1;
}

int rr_sdl_display_load_font(rrSDLDisplay* renderer, const char* path) {
    if (renderer->_font)
        TTF_CloseFont(renderer->_font);

    renderer->_font = TTF_OpenFont(path, 18);

    return renderer->_font != NULL;
}

void rr_sdl_display_init_score_text(rrSDLDisplay* renderer) {

    SDL_Color color={0,0,0};
    char scoreStrBuffer[32];
    snprintf(scoreStrBuffer, 32, "Score: %d", renderer->_game->player.score);
    SDL_Surface* text = TTF_RenderUTF8_Solid (renderer->_font, scoreStrBuffer, color);
    renderer->_scoreText = SDL_CreateTextureFromSurface(renderer->_renderer, text);
    SDL_FreeSurface(text);

    SDL_QueryTexture(renderer->_scoreText, NULL, NULL, &renderer->_scoreTextRect.w, &renderer->_scoreTextRect.h);
}

void rr_sdl_display_draw_board_background(rrSDLDisplay* renderer, rrPoint* map_pos) {
    SDL_Rect board_rect = {map_pos->x, map_pos->y, RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE, RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE};
    SDL_SetRenderDrawColor(renderer->_renderer, 195, 195, 0, 255);
    SDL_RenderFillRect(renderer->_renderer, &board_rect);
}

void rr_sdl_display_draw_map(rrSDLDisplay* renderer, rrPoint* map_pos) {
    rrPoint cell;

    SDL_Rect spriteTile;
    spriteTile.w = RR_RENDERER_TILE_SIZE;
    spriteTile.h = RR_RENDERER_TILE_SIZE;

    for (cell.x = 0; cell.x < RR_GRID_WIDTH; cell.x++) {
        for (cell.y = 0; cell.y < RR_GRID_HEIGHT; cell.y++) {
            unsigned char cell_value = rr_grid_get_cell(&renderer->_game->grid, &cell)->type;
            SpriteIndex sprite_index;

            if (cell_value == RR_CELL_EMPTY)
                continue;

            switch (cell_value) {
                case RR_CELL_BLOCK:
                    sprite_index = RR_SPRITE_BLOCK;
                    break;

                case RR_CELL_WALL:
                    sprite_index = RR_SPRITE_WALL;
                    break;

                default:
                    sprite_index = RR_SPRITE_COUNT;
            }

            assert(sprite_index != RR_SPRITE_COUNT);

            spriteTile.x = map_pos->x + cell.x * 16;
            spriteTile.y = map_pos->y + cell.y * 16;

            SDL_RenderCopy(renderer->_renderer, renderer->_spritesheet, renderer->_sprites + sprite_index, &spriteTile);
        }
    }
}

void rr_sdl_display_draw_entities(rrSDLDisplay* renderer, rrPoint* map_pos) {
    int i;
    SDL_Rect spriteTile;
    spriteTile.w = RR_RENDERER_TILE_SIZE;
    spriteTile.h = RR_RENDERER_TILE_SIZE;

    for (i = 0; i < MAX_ENEMIES; i++) {
        rrEnemy* enemy = &renderer->_game->_enemies[i];
        SpriteIndex sprite_index;

        if (enemy->entity.status == RR_STATUS_INACTIVE)
            continue;

        switch (enemy->entity.status) {
            case RR_STATUS_ACTIVE:
                sprite_index = RR_SPRITE_CAT;
                break;

            default:
                sprite_index = RR_SPRITE_COUNT;
        }

        assert(sprite_index != RR_SPRITE_COUNT);

        spriteTile.x = map_pos->x + enemy->entity.position.x * 16;
        spriteTile.y = map_pos->y + enemy->entity.position.y * 16;

        SDL_RenderCopy(renderer->_renderer, renderer->_spritesheet, renderer->_sprites + sprite_index, &spriteTile);
    }
}

void rr_sdl_display_draw_player(rrSDLDisplay* renderer, rrPoint* map_pos) {
    SDL_Rect spriteTile;
    rrPlayer* player = &renderer->_game->player;

    if (player->entity.status != RR_STATUS_ACTIVE)
        return;

    spriteTile.w = RR_RENDERER_TILE_SIZE;
    spriteTile.h = RR_RENDERER_TILE_SIZE;
    spriteTile.x = map_pos->x + player->entity.position.x * 16;
    spriteTile.y = map_pos->y + player->entity.position.y * 16;

    SDL_RenderCopy(renderer->_renderer, renderer->_spritesheet, renderer->_sprites + RR_SPRITE_MOUSE, &spriteTile);
}

void rr_sdl_display_draw(rrSDLDisplay* renderer) {
    int window_width, window_height;
    rrPoint map_pos;

    SDL_GetWindowSize(renderer->_window, &window_width, &window_height);
    map_pos.x = (window_width / 2)  - ((RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE) / 2);
    map_pos.y = (window_height / 2)  - ((RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE) / 2);

    if (!renderer->_scoreText){
        rr_sdl_display_init_score_text(renderer);
        renderer->_scoreTextRect.x = window_width - renderer->_scoreTextRect.w - 10;
        renderer->_scoreTextRect.y = 10;
    }

    SDL_SetRenderDrawColor(renderer->_renderer, 195, 195, 195, 255);
    SDL_RenderClear(renderer->_renderer);

    rr_sdl_display_draw_board_background(renderer, &map_pos);
    rr_sdl_display_draw_map(renderer, &map_pos);
    rr_sdl_display_draw_entities(renderer, &map_pos);
    rr_sdl_display_draw_player(renderer, &map_pos);

    SDL_RenderCopy(renderer->_renderer, renderer->_scoreText, NULL, &renderer->_scoreTextRect);

    SDL_RenderPresent(renderer->_renderer);
}