#ifndef rr_sdl_display_H
#define rr_sdl_display_H

#include "game.h"

#include <SDL.h>
#include <SDL_ttf.h>

typedef enum {
    RR_SPRITE_BLOCK,
    RR_SPRITE_WALL,
    RR_SPRITE_MOUSE,
    RR_SPRITE_CAT,
    RR_SPRITE_CAT_WAIT,
    RR_SPRITE_COUNT
} SpriteIndex;

typedef struct {
    rrGame* _game;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Rect _sprites[RR_SPRITE_COUNT];
    SDL_Texture* _spritesheet;
    TTF_Font* _font;
    SDL_Texture* _scoreText;
    SDL_Rect _scoreTextRect;

    SDL_Texture* _livesText;
    SDL_Rect _livesTextRect;

    rrPoint _map_pos;
    rrPoint window_size;
} rrSDLDisplay;

void rr_sdl_display_init(SDL_Window* window, rrSDLDisplay* display, rrGame* game);
void rr_sdl_display_uninit(rrSDLDisplay* display);
int rr_sdl_display_load_spritesheet(rrSDLDisplay* display, const char* path);
int rr_sdl_display_load_font(rrSDLDisplay* display, const char* path);
void rr_sdl_display_draw(rrSDLDisplay* display);

#endif
