#include <hal/debug.h>
#include <hal/xbox.h>
#include <hal/video.h>
#include <windows.h>

#include "../sdl/sdl_game.h"
#include "../sdl/sdl_input.h"

#include <SDL.h>

#define RR_XBOX_SCREEN_WIDTH 640
#define RR_XBOX_SCREEN_HEIGHT 480

static void xbox_fatal_error(const char* description)
{
    debugPrint("Fatal Error: %s\n", description);
    Sleep(10000);
    XReboot();
}

int main() {
    rrSDLGame* game;
    rrInput* input;

    XVideoSetMode(RR_XBOX_SCREEN_WIDTH, RR_XBOX_SCREEN_HEIGHT, 32, REFRESH_DEFAULT);

    /* Note that D:/ is mounted as cwd when game is launched. */
    game = rr_sdl_game_create("D:\\assets");

    if (!rr_sdl_game_init(game, RR_XBOX_SCREEN_WIDTH, RR_XBOX_SCREEN_HEIGHT))
        xbox_fatal_error(rr_sdl_game_get_error_str(game));

    /* nxdk implementation of SDL requires this hint to be set in order to actually process controller input events. */
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    input = rr_sdl_game_get_input(game);
    if (!rr_sdl_input_controller_active(input)) {
        xbox_fatal_error("Unable to initialize controller.");
    }

    rr_sdl_game_run(game);
    rr_sdl_game_destroy(game);

    return 0;
}
