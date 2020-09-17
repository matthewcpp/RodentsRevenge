#include "../sdl/sdl_game.h"

#include <stdio.h>

#define RR_SCREEN_WIDTH 640
#define RR_SCREEN_HEIGHT 480

int main(){
    rrSDLGame* sdl_game;

    sdl_game = rr_sdl_game_create(ASSET_DIRECTORY);
    if (!rr_sdl_game_init(sdl_game, RR_SCREEN_WIDTH, RR_SCREEN_HEIGHT)) {
        printf("Error Initializing game: %s\n", rr_sdl_game_get_error_str(sdl_game));
        return 1;
    }

    rr_sdl_game_run(sdl_game);
    rr_sdl_game_destroy(sdl_game);

    return 0;
}
