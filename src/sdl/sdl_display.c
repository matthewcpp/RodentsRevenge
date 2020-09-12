#include "sdl_display.h"
#include "sdl_animation.h"
#include "defs.h"

#include <SDL_image.h>

#include <assert.h>

#define RR_RENDERER_TILE_SIZE 16

typedef enum {
    RR_SPRITE_BLOCK,
    RR_SPRITE_WALL,
    RR_SPRITE_MOUSE,
    RR_SPRITE_CAT,
    RR_SPRITE_CAT_WAIT,
    RR_SPRITE_CHEESE,
    RR_SPRITE_REMAINING_LIFE,
    RR_SPRITE_COUNT
} SpriteIndex;

struct rrSDLDisplay{
    rrGame* _game;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Rect _sprites[RR_SPRITE_COUNT];
    SDL_Texture* _spritesheet;
    TTF_Font* _font;

    int _previous_score;
    SDL_Texture* _scoreText;
    SDL_Rect _scoreTextRect;

    SDL_Texture* _livesText;
    SDL_Rect _livesTextRect;

    rrPoint _map_pos;
    rrPoint window_size;
};

void rr_sdl_display_setup_display_elements(rrSDLDisplay* display);
void rr_sdl_display_init_score_text(rrSDLDisplay* display);
void rr_sdl_display_init_lives_text(rrSDLDisplay* display);

rrSDLDisplay* rr_sdl_display_create(SDL_Window* window, rrGame* game) {
    rrSDLDisplay* display = malloc(sizeof(rrSDLDisplay));

    display->_window = window;
    display->_renderer = SDL_CreateRenderer(window, -1, 0);
    display->_game = game;
    display->_spritesheet = NULL;
    display->_font = NULL;
    display->_previous_score = 0;
    display->_scoreText = NULL;
    display->_livesText = NULL;

    TTF_Init();

    return display;
}

void rr_sdl_display_destroy(rrSDLDisplay* display) {
    rr_sdl_animation_destroy(display->_game->player.death_animation);

    SDL_DestroyTexture(display->_scoreText);
    SDL_DestroyTexture(display->_livesText);
    SDL_DestroyTexture(display->_spritesheet);
    SDL_DestroyRenderer(display->_renderer);

    if (display->_font)
        TTF_CloseFont(display->_font);

    TTF_Quit();

    free(display);
}

void rr_sdl_display_setup_display_elements(rrSDLDisplay* display){
    SDL_GetRendererOutputSize(display->_renderer, &display->window_size.x, &display->window_size.y);
    display->_map_pos.x = (display->window_size.x / 2)  - ((RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE) / 2);
    display->_map_pos.y = (display->window_size.y / 2)  - ((RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE) / 2);

    rr_sdl_display_init_score_text(display);
    rr_sdl_display_init_lives_text(display);
}

void rr_sdl_display_sprite_info(SDL_Rect* rect, int x, int y) {
    rect->x = x;
    rect->y = y;
    rect->w = 16;
    rect->h = 16;
}

void rr_sdl_create_player_death_anim(rrSDLDisplay* display) {
    rrPoint animation_frames[] = {{18, 36}, {54, 54}, {36, 36}, {0, 18}, {18, 18}};
    rrPoint frame_size = {16, 16};
    display->_game->player.death_animation = rr_sdl_animation_create(display->_spritesheet, 5, &animation_frames[0], &frame_size, 100);
}

int rr_sdl_display_load_spritesheet(rrSDLDisplay* display, const char* path) {
    SDL_Surface* surface = IMG_Load(path);

    if (!surface)
        return 0;

    display->_spritesheet = SDL_CreateTextureFromSurface(display->_renderer, surface);
    SDL_FreeSurface(surface);

    rr_sdl_display_sprite_info(display->_sprites + RR_SPRITE_BLOCK, 18, 0);
    rr_sdl_display_sprite_info(display->_sprites + RR_SPRITE_MOUSE, 54, 34);
    rr_sdl_display_sprite_info(display->_sprites + RR_SPRITE_WALL, 36, 0);
    rr_sdl_display_sprite_info(display->_sprites + RR_SPRITE_CAT, 85, 52);
    rr_sdl_display_sprite_info(display->_sprites + RR_SPRITE_CAT_WAIT, 0, 36);
    rr_sdl_display_sprite_info(display->_sprites + RR_SPRITE_CHEESE, 36, 54);
    rr_sdl_display_sprite_info(display->_sprites + RR_SPRITE_REMAINING_LIFE, 36, 18);

    rr_sdl_create_player_death_anim(display);

    return 1;
}

int rr_sdl_display_load_font(rrSDLDisplay* display, const char* path) {
    if (display->_font)
        TTF_CloseFont(display->_font);

    display->_font = TTF_OpenFont(path, 18);

    return display->_font != NULL;
}

void rr_sdl_display_init_score_text(rrSDLDisplay* display) {
    SDL_Surface* text = NULL;
    SDL_Color color={0,0,0};
    char scoreStrBuffer[32];
    snprintf_func(scoreStrBuffer, 32, "Score: %d", display->_game->player.score);
    text = TTF_RenderUTF8_Solid (display->_font, scoreStrBuffer, color);
    display->_scoreText = SDL_CreateTextureFromSurface(display->_renderer, text);
    SDL_FreeSurface(text);

    SDL_QueryTexture(display->_scoreText, NULL, NULL, &display->_scoreTextRect.w, &display->_scoreTextRect.h);
    display->_scoreTextRect.x = display->window_size.x - display->_scoreTextRect.w - 10;
    display->_scoreTextRect.y = 10;
}

void rr_sdl_display_init_lives_text(rrSDLDisplay* display) {
    SDL_Color color={0,0,0};
    SDL_Surface* text = TTF_RenderUTF8_Solid (display->_font, "Lives:", color);

    if (display->_livesText)
        SDL_DestroyTexture(display->_spritesheet);

    display->_livesText = SDL_CreateTextureFromSurface(display->_renderer, text);
    SDL_FreeSurface(text);

    SDL_QueryTexture(display->_livesText, NULL, NULL, &display->_livesTextRect.w, &display->_livesTextRect.h);
    display->_livesTextRect.x = 10;
    display->_livesTextRect.y = 10;
}

void rr_sdl_display_draw_board_background(rrSDLDisplay* display) {
    SDL_Rect board_rect;
    board_rect.x = display->_map_pos.x;
    board_rect.y = display->_map_pos.y;
    board_rect.w = RR_GRID_WIDTH * RR_RENDERER_TILE_SIZE;
    board_rect.h = RR_GRID_HEIGHT * RR_RENDERER_TILE_SIZE;

    SDL_SetRenderDrawColor(display->_renderer, 195, 195, 0, 255);
    SDL_RenderFillRect(display->_renderer, &board_rect);
}

void rr_sdl_display_set_sprite_rect(rrSDLDisplay* display, SDL_Rect* sprite_tile, rrPoint* position) {
    sprite_tile->w = RR_RENDERER_TILE_SIZE;
    sprite_tile->h = RR_RENDERER_TILE_SIZE;
    sprite_tile->x = display->_map_pos.x + position->x * 16;
    sprite_tile->y = display->_map_pos.y + position->y * 16;
}

void rr_sdl_display_draw_basic_block(rrSDLDisplay* display, SpriteIndex sprite, rrPoint* position) {
    SDL_Rect spriteTile;
    rr_sdl_display_set_sprite_rect(display, &spriteTile, position);
    SDL_RenderCopy(display->_renderer, display->_spritesheet, display->_sprites + sprite, &spriteTile);
}

void rr_sdl_display_draw_enemy(rrSDLDisplay* display, rrEnemy* enemy) {
    SDL_Rect sprite_tile;
    SpriteIndex sprite_index;

    switch (enemy->entity.status) {
        case RR_STATUS_ACTIVE:
            sprite_index = RR_SPRITE_CAT;
            break;

        case RR_STATUS_WAITING:
            sprite_index = RR_SPRITE_CAT_WAIT;
            break;

        default:
            return;
    }

    rr_sdl_display_set_sprite_rect(display, &sprite_tile, &enemy->entity.position);
    SDL_RenderCopy(display->_renderer, display->_spritesheet, display->_sprites + sprite_index, &sprite_tile);
}

void rr_sdl_display_draw_player(rrSDLDisplay* display, rrPlayer* player) {
    SDL_Rect* sprite_src_rect;
    SDL_Rect texture_dest_rect;
    SDL_Texture* texture;

    switch (player->entity.status) {
        case RR_STATUS_ACTIVE:
            texture = display->_spritesheet;
            sprite_src_rect = display->_sprites + RR_SPRITE_MOUSE;
            break;

        case RR_STATUS_DYING:
            texture = player->death_animation->texture;
            sprite_src_rect = &player->death_animation->current_frame_rect;
            break;

        default:
            return;
    }

    rr_sdl_display_set_sprite_rect(display, &texture_dest_rect, &player->entity.position);
    SDL_RenderCopy(display->_renderer, texture, sprite_src_rect, &texture_dest_rect);
}

void rr_sdl_display_draw_lives(rrSDLDisplay* display) {
    int i;
    int adjustment = (display->_livesTextRect.h - RR_RENDERER_TILE_SIZE) / 2;
    SDL_Rect sprite_rect;
    sprite_rect.x = display->_livesTextRect.x + display->_livesTextRect.w + 5;
    sprite_rect.y = 10 + adjustment;
    sprite_rect.w = RR_RENDERER_TILE_SIZE;
    sprite_rect.h = RR_RENDERER_TILE_SIZE;

    SDL_RenderCopy(display->_renderer, display->_livesText, NULL, &display->_livesTextRect);

    for (i = 0; i < display->_game->player.lives_remaining; i++) {
        SDL_RenderCopy(display->_renderer, display->_spritesheet, display->_sprites + RR_SPRITE_REMAINING_LIFE, &sprite_rect);
        sprite_rect.x += RR_RENDERER_TILE_SIZE + 5;
    }
}

void rr_sdl_display_draw_entities(rrSDLDisplay* display) {
    rrPoint cell;
    for (cell.x = 0; cell.x < RR_GRID_WIDTH; cell.x++) {
        for (cell.y = 0; cell.y < RR_GRID_HEIGHT; cell.y++) {
            rrEntity* entity = rr_grid_get_entity_at_position(display->_game->grid, &cell);

            if (!entity)
                continue;

            switch (entity->type) {
                case RR_ENTITY_WALL:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITE_WALL, &cell);
                    break;

                case RR_ENTITY_BLOCK:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITE_BLOCK, &cell);
                    break;

                case RR_ENTITY_CHEESE:
                    rr_sdl_display_draw_basic_block(display, RR_SPRITE_CHEESE, &cell);
                    break;

                case RR_ENTITY_ENEMY:
                    rr_sdl_display_draw_enemy(display, (rrEnemy*)entity);
                    break;

                case RR_ENTITY_PLAYER:
                    rr_sdl_display_draw_player(display, (rrPlayer*)entity);
                    break;
            }
        }
    }
}

void rr_sdl_display_draw(rrSDLDisplay* display) {
    if (!display->_scoreText)
        rr_sdl_display_setup_display_elements(display);

    SDL_SetRenderDrawColor(display->_renderer, 195, 195, 195, 255);
    SDL_RenderClear(display->_renderer);

    rr_sdl_display_draw_board_background(display);
    rr_sdl_display_draw_entities(display);

    if (display->_game->player.score != display->_previous_score){
        rr_sdl_display_init_score_text(display);
        display->_previous_score = display->_game->player.score;
    }

    SDL_RenderCopy(display->_renderer, display->_scoreText, NULL, &display->_scoreTextRect);
    rr_sdl_display_draw_lives(display);

    SDL_RenderPresent(display->_renderer);
}