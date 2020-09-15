#include "../input.h"
#include "../game.h"

typedef struct rrSDLGame rrSDLGame;

rrSDLGame* rr_sdl_game_create(const char* asset_path);
int rr_sdl_game_init(rrSDLGame* game, int screen_width, int screen_height);
rrInput* rr_sdl_game_get_input(rrSDLGame* game);
void rr_sdl_game_run(rrSDLGame* game);
void rr_sdl_game_destroy(rrSDLGame* game);
const char* rr_sdl_game_get_error_str(rrSDLGame* game);

/** Gets the platform independent game object. */
rrGame* rr_sdl_game_get_base_game(rrSDLGame* sdl_game);
