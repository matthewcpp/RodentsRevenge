#include "defs.h"
#include "sdl/sdl_game.h"

#define RR_SCREEN_WIDTH 640
#define RR_SCREEN_HEIGHT 480

int main(int argc, char* argv[]){
    rrSDLGame* game;
    char asset_path[256];

    snprintf_func(asset_path, 256, "%s/", ASSET_DIRECTORY);

    game = rr_sdl_game_create(asset_path);
    rr_sdl_game_init(game, RR_SCREEN_WIDTH, RR_SCREEN_HEIGHT);
    rr_sdl_game_run(game);
    rr_sdl_game_destroy(game);

    return 0;
}
