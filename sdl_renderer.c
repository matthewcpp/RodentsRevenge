#include "sdl_renderer.h"

#include <SDL_image.h>

#include <stdio.h>

#define RR_RENDERER_TILE_SIZE 16

void rr_sdl_renderer_init(SDL_Window* window, rrSDLRenderer* renderer, rrGame* game) {
    renderer->_window = window;
    renderer->_renderer = SDL_CreateRenderer(window, -1, 0);
    renderer->_game = game;
    renderer->_spritesheet = NULL;
    renderer->_font = NULL;
    renderer->_scoreText = NULL;

    TTF_Init();
}

void rr_sdl_renderer_uninit(rrSDLRenderer* renderer) {
    SDL_DestroyTexture(renderer->_spritesheet);
    SDL_DestroyRenderer(renderer->_renderer);

    if (renderer->_font)
        TTF_CloseFont(renderer->_font);

    TTF_Quit();
}

void rr_sdl_renderer_sprite_info(SDL_Rect* rect, int x, int y) {
    rect->x = x;
    rect->y = y;
    rect->w = 16;
    rect->h = 16;
}

int rr_sdl_renderer_load_spritesheet(rrSDLRenderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);

    if (!surface)
        return 0;

    renderer->_spritesheet = SDL_CreateTextureFromSurface(renderer->_renderer, surface);
    SDL_FreeSurface(surface);

    rr_sdl_renderer_sprite_info(renderer->_sprites + RR_CELL_BLOCK, 1, 1);
    rr_sdl_renderer_sprite_info(renderer->_sprites + RR_CELL_MOUSE, 37, 19);
    rr_sdl_renderer_sprite_info(renderer->_sprites + RR_CELL_WALL, 37, 37);

    return 1;
}

int rr_sdl_renderer_load_font(rrSDLRenderer* renderer, const char* path) {
    if (renderer->_font)
        TTF_CloseFont(renderer->_font);

    renderer->_font = TTF_OpenFont(path, 18);

    return renderer->_font != NULL;
}

void rr_sdl_renderer_init_score_text(rrSDLRenderer* renderer) {

    SDL_Color color={0,0,0};
    char scoreStrBuffer[32];
    snprintf(scoreStrBuffer, 32, "Score: %d", renderer->_game->player.score);
    SDL_Surface* text = TTF_RenderUTF8_Solid (renderer->_font, scoreStrBuffer, color);
    renderer->_scoreText = SDL_CreateTextureFromSurface(renderer->_renderer, text);
    SDL_FreeSurface(text);

    SDL_QueryTexture(renderer->_scoreText, NULL, NULL, &renderer->_scoreTextRect.w, &renderer->_scoreTextRect.h);
}

void rr_sdl_renderer_draw_sprites(rrSDLRenderer* renderer, int map_x, int map_y) {
    int x, y;

    SDL_Rect spriteTile;
    spriteTile.w = RR_RENDERER_TILE_SIZE;
    spriteTile.h = RR_RENDERER_TILE_SIZE;

    for (x = 0; x < RR_GRID_WIDTH; x++) {
        for (y = 0; y < RR_GRID_HEIGHT; y++) {
            int index = y * RR_GRID_WIDTH + x;
            char cellValue = renderer->_game->grid.cells[index];

            spriteTile.x = map_x + x * 16;
            spriteTile.y = map_y + y * 16;

            if (cellValue == RR_CELL_EMPTY)
                continue;

            SDL_RenderCopy(renderer->_renderer, renderer->_spritesheet, renderer->_sprites + cellValue, &spriteTile);
        }
    }
}

void rr_sdl_renderer_draw_board_background(rrSDLRenderer* renderer, int map_x, int map_y) {
    SDL_Rect board_rect = {map_x, map_y, RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE, RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE};
    SDL_SetRenderDrawColor(renderer->_renderer, 195, 195, 0, 255);
    SDL_RenderFillRect(renderer->_renderer, &board_rect);
}

void rr_sdl_renderer_draw(rrSDLRenderer* renderer) {
    int window_width, window_height;
    int map_x, map_y;

    SDL_GetWindowSize(renderer->_window, &window_width, &window_height);
    map_x = (window_width / 2)  - ((RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE) / 2);
    map_y = (window_height / 2)  - ((RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE) / 2);

    if (!renderer->_scoreText){
        rr_sdl_renderer_init_score_text(renderer);
        renderer->_scoreTextRect.x = window_width - renderer->_scoreTextRect.w - 10;
        renderer->_scoreTextRect.y = 10;
    }

    SDL_SetRenderDrawColor(renderer->_renderer, 195, 195, 195, 255);
    SDL_RenderClear(renderer->_renderer);

    rr_sdl_renderer_draw_board_background(renderer, map_x, map_y);
    rr_sdl_renderer_draw_sprites(renderer, map_x, map_y);

    SDL_RenderCopy(renderer->_renderer, renderer->_scoreText, NULL, &renderer->_scoreTextRect);

    SDL_RenderPresent(renderer->_renderer);
}