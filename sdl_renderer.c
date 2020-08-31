#include "sdl_renderer.h"

#include <SDL_image.h>


void rr_sdl_renderer_init(rrSDLRenderer* renderer, rrGame* game, SDL_Window* window) {
    renderer->_renderer = SDL_CreateRenderer(window, -1, 0);
    renderer->_game = game;
    renderer->_spritesheet = NULL;
}

void rr_sdl_renderer_sprite_info(SDL_Rect* rect, int x, int y) {
    rect->x = x;
    rect->y = y;
    rect->w = 16;
    rect->h = 16;
}

int rr_sdl_renderer_load_spritesheet(rrSDLRenderer* renderer, const char* path) {
    SDL_Surface* surface =IMG_Load(path);

    if (!surface)
        return 0;

    renderer->_spritesheet = SDL_CreateTextureFromSurface(renderer->_renderer, surface);
    SDL_FreeSurface(surface);

    rr_sdl_renderer_sprite_info(renderer->_sprites + RR_CELL_BLOCK, 1, 1);
    rr_sdl_renderer_sprite_info(renderer->_sprites + RR_CELL_MOUSE, 37, 19);
    rr_sdl_renderer_sprite_info(renderer->_sprites + RR_CELL_WALL, 37, 37);

    return 1;
}

void rr_sdl_renderer_draw(rrSDLRenderer* renderer) {
    int x, y;
    SDL_Rect tile;
    tile.w = 16;
    tile.h = 16;
    SDL_SetRenderDrawColor(renderer->_renderer, 195, 195, 0, 255);
    SDL_RenderClear(renderer->_renderer);

    for (x = 0; x < RR_GRID_WIDTH; x++) {
        for (y = 0; y < RR_GRID_HEIGHT; y++) {
            int index = y * RR_GRID_WIDTH + x;
            char cellValue = renderer->_game->grid.cells[index];

            tile.x = x * 16;
            tile.y = y * 16;

            if (cellValue == RR_CELL_EMPTY)
                continue;

            SDL_RenderCopy(renderer->_renderer, renderer->_spritesheet, renderer->_sprites + cellValue, &tile);
        }
    }

    SDL_RenderPresent(renderer->_renderer);
}