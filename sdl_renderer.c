#include "sdl_renderer.h"

void rr_sdl_renderer_init(rrSDLRenderer* renderer, rrGame* game, SDL_Window* window) {
    renderer->_renderer = SDL_CreateRenderer(window, -1, 0);
    renderer->_game = game;
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

            tile.x = x * 16;
            tile.y = y * 16;

            switch (renderer->_game->grid.cells[index]) {
                case RR_CELL_BLOCK:
                    SDL_SetRenderDrawColor(renderer->_renderer, 0, 195, 0, 255);
                    break;

                case RR_CELL_WALL:
                    SDL_SetRenderDrawColor(renderer->_renderer, 0, 195, 195, 255);
                    break;

                case RR_CELL_MOUSE:
                    SDL_SetRenderDrawColor(renderer->_renderer, 255, 0, 0, 255);
                    break;

                default:
                    continue;
            }

            SDL_RenderFillRect(renderer->_renderer, &tile);
        }
    }

    SDL_RenderPresent(renderer->_renderer);
}