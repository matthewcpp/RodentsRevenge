#include <hal/debug.h>
#include <hal/xbox.h>
#include <hal/video.h>
#include <windows.h>

#include "../sdl/sdl_game.h"

#define RR_XBOX_SCREEN_WIDTH 640
#define RR_XBOX_SCREEN_HEIGHT 480

static void xbox_fatal_error(const char* description)
{
    debugPrint("SDL_Error: %s\n", description);
    Sleep(10000);
    XReboot();
}

int main() {
    rrSDLGame* game;

    XVideoSetMode(RR_XBOX_SCREEN_WIDTH, RR_XBOX_SCREEN_HEIGHT, 32, REFRESH_DEFAULT);

    game = rr_sdl_game_create("D:\\assets\\");

    if (!rr_sdl_game_init(game, RR_XBOX_SCREEN_WIDTH, RR_XBOX_SCREEN_HEIGHT))
        xbox_fatal_error(rr_sdl_game_get_error_str(game));

    rr_sdl_game_run(game);
    rr_sdl_game_destroy(game);

    return 0;
}
