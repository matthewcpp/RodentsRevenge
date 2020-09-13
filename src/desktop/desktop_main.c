#include "../sdl/sdl_game.h"

#include <stdio.h>

#define RR_SCREEN_WIDTH 640
#define RR_SCREEN_HEIGHT 480

int main(){
    rrSDLGame* game;

    game = rr_sdl_game_create(ASSET_DIRECTORY);
    if (!rr_sdl_game_init(game, RR_SCREEN_WIDTH, RR_SCREEN_HEIGHT)) {
        printf("Error Initializing game: %s\n", rr_sdl_game_get_error_str(game));
        return 1;
    }

    rr_sdl_game_run(game);
    rr_sdl_game_destroy(game);

    return 0;
}
