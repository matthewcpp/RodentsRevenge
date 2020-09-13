#include <hal/debug.h>
#include <hal/video.h>

#include "../sdl/sdl_game.h"

const extern int SCREEN_WIDTH;
const extern int SCREEN_HEIGHT;

int main() {
    rrSDLGame* game;

    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    game = rr_sdl_game_create("assets/");
    rr_game_init(game, SCREEN_WIDTH, SCREEN_HEIGHT);
    rr_sdl_game_run(game);
    rr_sdl_game_destroy(game);

    return 0;
}
