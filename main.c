#include "game.h"

#include <SDL.h>
#include "sdl_controller.h"
#include "sdl_renderer.h"

#include "stdlib.h"
#include "stdio.h"

#define RR_SCREEN_WIDTH 640
#define RR_SCREEN_HEIGHT 480

int main(int argc, char* argv[]){
    SDL_Window* window = NULL;
    SDL_Event event;
    int keep_going = 1;
    int i;
    Uint32 last_update;

    rrGame* game = NULL;
    rrSDLController controller;
    rrSDLRenderer renderer;
    (void)argc;
    (void)argv;

    game = malloc(sizeof(rrGame));

    SDL_VideoInit(NULL);
    window = SDL_CreateWindow("RodentsRevenge",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              RR_SCREEN_WIDTH,
                              RR_SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    if (SDL_NumJoysticks() < 1) {
        puts("No joysticks detected");
    }

    rr_sdl_controller_init(&controller, game);
    rr_sdl_renderer_init(window, &renderer, game);
    rr_sdl_renderer_load_spritesheet(&renderer, "C:/development/repos/RodentsRevenge/assets/spritesheet.png");
    rr_sdl_renderer_load_font(&renderer, "C:/development/repos/RodentsRevenge/assets/vegur-regular.ttf");

    rr_game_init(game);

    rr_player_set_pos(&game->player, 3, 3);

    for (i = 0; i < RR_GRID_WIDTH; i++) {
        rr_grid_set_cell_type(&game->grid, i,0, RR_CELL_WALL);
        rr_grid_set_cell_type(&game->grid, i,RR_GRID_HEIGHT - 1, RR_CELL_WALL);
    }

    for (i = 0; i < RR_GRID_HEIGHT; i++) {
        rr_grid_set_cell_type(&game->grid, 0, i, RR_CELL_WALL);
        rr_grid_set_cell_type(&game->grid, RR_GRID_WIDTH - 1, i, RR_CELL_WALL);
    }

    rr_grid_set_cell_type(&game->grid, 5,5, RR_CELL_BLOCK);
    rr_grid_set_cell_type(&game->grid, 6,5, RR_CELL_BLOCK);

    last_update =  SDL_GetTicks();


    while (keep_going) {
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    keep_going = 0;
                    break;
            }
        }

        Uint32 now = SDL_GetTicks();
        if (now - last_update >= 32) {
            rr_sdl_controller_update(&controller);
            rr_sdl_renderer_draw(&renderer);
            last_update = now;
        }
        SDL_Delay(1);
    }

    rr_game_uninit(game);
    rr_sdl_renderer_uninit(&renderer);
    SDL_VideoQuit();

    return 0;
}