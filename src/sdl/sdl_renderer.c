#include "sdl_renderer.h"
#include "../util.h"

#include <SDL_image.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

rrRenderer* rr_sdl_renderer_create(SDL_Window* window) {
    rrRenderer* renderer = malloc(sizeof(rrRenderer));
    renderer->renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_GetRendererOutputSize(renderer->renderer, &renderer->screen_size.x, &renderer->screen_size.y);

    renderer->text_sprites = cutil_vector_create(cutil_trait_ptr());
    renderer->sprites = calloc(RR_SPRITE_COUNT, sizeof(rrSprite*));
    renderer->fonts = calloc(RR_FONT_COUNT, sizeof(TTF_Font*));
    renderer->animations = calloc(RR_ANIMATION_COUNT, sizeof(rrAnimation*));
    renderer->spritesheet = NULL;

    TTF_Init();

    return renderer;
}

void rr_sdl_renderer_destroy(rrRenderer* renderer) {
    rrSprite* sprite;
    size_t count = cutil_vector_size(renderer->text_sprites);
    size_t i;

    for (i = 0; i < count; i++) {
        cutil_vector_get(renderer->text_sprites, i, &sprite);
        SDL_DestroyTexture(sprite->handle);
        free(sprite);
    }

    for (i = 0; i < RR_SPRITE_COUNT; i++) {
        sprite = renderer->sprites[i];
        if (sprite){
            SDL_DestroyTexture(sprite->handle);
            free(sprite);
        }
    }

    for (i = 0; i < RR_FONT_COUNT; i++) {
        TTF_Font* font = renderer->fonts[i];
        if (font)
            TTF_CloseFont(font);
    }

    for (i = 0; i < RR_ANIMATION_COUNT; i++) {
        rrAnimation * animation = renderer->animations[i];
        if (animation) {
            rr_animation_destroy(animation);
        }
    }

    if (renderer->spritesheet)
        rr_spritesheet_destroy(renderer->spritesheet);

    free(renderer->sprites);
    free(renderer->fonts);
    free(renderer->animations);

    cutil_vector_destroy(renderer->text_sprites);

    SDL_DestroyRenderer(renderer->renderer);

    TTF_Quit();
}

void rr_sdl_renderer_begin(rrRenderer* renderer) {
    SDL_SetRenderDrawColor(renderer->renderer, 195, 195, 195, 255);
    SDL_RenderClear(renderer->renderer);
}

void rr_sdl_renderer_end(rrRenderer* renderer) {
    SDL_RenderPresent(renderer->renderer);
}

void rr_renderer_get_screen_size(rrRenderer* renderer, rrPoint* size) {
    rr_point_copy(size, &renderer->screen_size);
}

void rr_renderer_color(struct rrRenderer* renderer, rrColor* color) {
    SDL_SetRenderDrawColor(renderer->renderer, color->r, color->g, color->b, color->a);
}

void rr_renderer_fill_rect(rrRenderer* renderer, rrRect* rect) {
    SDL_RenderFillRect(renderer->renderer, (SDL_Rect*)rect);
}

void rr_renderer_draw_rect(rrRenderer* renderer, rrRect* rect) {
    SDL_RenderDrawRect(renderer->renderer, (SDL_Rect*)rect);
}

rrSprite* rr_renderer_load_sprite(rrRenderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    SDL_Texture* texture;
    rrSprite* sprite;

    if (!surface)
        return NULL;

    texture = SDL_CreateTextureFromSurface(renderer->renderer, surface);
    SDL_FreeSurface(surface);

    sprite = malloc(sizeof(rrSprite));
    sprite->handle = texture;

    SDL_QueryTexture(sprite->handle, NULL, NULL, &sprite->rect.w, &sprite->rect.h);
    sprite->rect.x = 0;
    sprite->rect.y = 0;

    return sprite;
}

rrAnimation* rr_renderer_create_animation(rrRenderer* renderer, int index, int frame_count, rrSprite** frames, int frame_time) {
    assert(index < RR_ANIMATION_COUNT);
    if (renderer->animations[index])
        rr_animation_destroy(renderer->animations[index]);

    renderer->animations[index] = rr_animation_create(frame_count, frames, frame_time);

    return renderer->animations[index];
}

void rr_renderer_create_text_texture(rrRenderer* renderer, rrSprite* text_sprite, int font, const char* text) {
    SDL_Surface* text_surface = NULL;
    SDL_Color color={255,255,255, 255};

    (void)font;
    text_surface = TTF_RenderUTF8_Solid (renderer->fonts[font], text, color);
    text_sprite->handle = SDL_CreateTextureFromSurface(renderer->renderer, text_surface);
    SDL_FreeSurface(text_surface);
    SDL_QueryTexture(text_sprite->handle, NULL, NULL, &text_sprite->rect.w, &text_sprite->rect.h);
    text_sprite->rect.x = 0;
    text_sprite->rect.y = 0;
}

rrSprite* rr_renderer_create_text(rrRenderer* renderer, int font, const char* text) {
    rrSprite* text_sprite = malloc(sizeof(rrSprite));
    rr_renderer_create_text_texture(renderer, text_sprite, font, text);
    cutil_vector_push_back(renderer->text_sprites, &text_sprite);

    return text_sprite;
}

void rr_renderer_update_text_sprite(rrRenderer* renderer, rrSprite* text_sprite, int font, const char* text) {
    SDL_Color color;
    SDL_GetTextureColorMod(text_sprite->handle, &color.r, &color.g, &color.b);
    SDL_DestroyTexture(text_sprite->handle);
    rr_renderer_create_text_texture(renderer, text_sprite, font, text);
    SDL_SetTextureColorMod(text_sprite->handle, color.r, color.g, color.b);
}

void rr_renderer_draw_sprite(rrRenderer* renderer, rrSprite* sprite, rrPoint* position) {
    SDL_Rect dest_rect;
    dest_rect.x = position->x;
    dest_rect.y = position->y;
    dest_rect.w = sprite->rect.w;
    dest_rect.h = sprite->rect.h;

    SDL_RenderCopy(renderer->renderer, sprite->handle, (SDL_Rect*)&sprite->rect, &dest_rect);
}

void rr_renderer_set_sprite_tint_color(rrRenderer* renderer, rrSprite* sprite, rrColor* color) {
    (void)renderer;
    SDL_SetTextureColorMod(sprite->handle, color->r, color->g, color->b);
}

void rr_renderer_draw_line(rrRenderer* renderer, rrPoint* a, rrPoint* b) {
    SDL_RenderDrawLine(renderer->renderer, a->x, a->y, b->x, b->y);
}

rrSprite* rr_renderer_get_sprite(rrRenderer* renderer, int index) {
    if (index >= RR_SPRITE_COUNT || index < 0)
        return NULL;

    return renderer->sprites[index];
}

rrAnimation* rr_renderer_get_animation(rrRenderer* renderer, int index) {
    assert(index < RR_ANIMATION_COUNT);
    return renderer->animations[index];
}

void rr_sdl_renderer_spritesheet_add_tile(rrRenderer* renderer, rrSpriteSheetIndex index, int x, int y) {
    rrRect rect;
    rect.x = x;
    rect.y = y;
    rect.w = 16;
    rect.h = 16;

    rr_spritesheet_add_sprite(renderer->spritesheet, index, &rect);
}

void rr_sdl_renderer_init_spritesheet(rrRenderer* renderer) {
    renderer->spritesheet = rr_spritesheet_create(renderer->sprites[RR_SPRITE_SPRITESHEET], RR_SPRITESHEET_INDEX_COUNT);

    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_BLOCK, 1, 1);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_MOUSE, 37, 37);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_WALL, 37, 55);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_CAT, 19, 1);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_CAT_WAIT, 37, 1);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_CHEESE, 55, 1);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_REMAINING_LIFE, 55, 37);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_HOLE, 19, 37);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_STUCK_PLAYER, 19, 55);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_MOUSETRAP, 1, 55);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_PLAYER_DEATH1, 1, 19);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_PLAYER_DEATH2, 19, 19);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_PLAYER_DEATH3, 37, 19);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_PLAYER_DEATH4, 55, 19);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_PLAYER_DEATH5, 1, 37);

    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_YARN, 55, 55);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_YARN_EXPLODE1, 73, 1);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_YARN_EXPLODE2, 73, 15);
    rr_sdl_renderer_spritesheet_add_tile(renderer, RR_SPRITESHEET_INDEX_YARN_EXPLODE3, 73, 33);
}

void rr_sdl_display_init_animations(rrRenderer* renderer) {
    rrSprite* animation_frames[5];

    animation_frames[0] = renderer->spritesheet->sprites[RR_SPRITESHEET_INDEX_PLAYER_DEATH1];
    animation_frames[1] = renderer->spritesheet->sprites[RR_SPRITESHEET_INDEX_PLAYER_DEATH2];
    animation_frames[2] = renderer->spritesheet->sprites[RR_SPRITESHEET_INDEX_PLAYER_DEATH3];
    animation_frames[3] = renderer->spritesheet->sprites[RR_SPRITESHEET_INDEX_PLAYER_DEATH4];
    animation_frames[4] = renderer->spritesheet->sprites[RR_SPRITESHEET_INDEX_PLAYER_DEATH5];

    rr_renderer_create_animation(renderer, RR_ANIMATION_PLAYER_DEATH, 5, animation_frames, 100);
}


int rr_sdl_renderer_load_sprites(rrRenderer* renderer, const char* asset_dir) {
    rrSprite* sprite;
    char asset_path[256];
    sprintf(asset_path, "%s%s%s", asset_dir, rr_path_sep(), "spritesheet.png");

    sprite = rr_renderer_load_sprite(renderer, asset_path);
    if (sprite) {
        renderer->sprites[RR_SPRITE_SPRITESHEET] = sprite;
        rr_sdl_renderer_init_spritesheet(renderer);
        rr_sdl_display_init_animations(renderer);
    }
    else {
        return 0;
    }

    sprintf(asset_path, "%s%s%s", asset_dir, rr_path_sep(), "logo.png");
    sprite = rr_renderer_load_sprite(renderer, asset_path);

    if (sprite)
        renderer->sprites[RR_SPRITE_TITLE_LOGO] = sprite;
    else
        return 0;

    sprintf(asset_path, "%s%s%s", asset_dir, rr_path_sep(), "clock.png");
    sprite = rr_renderer_load_sprite(renderer, asset_path);

    if (sprite)
        renderer->sprites[RR_SPRITE_CLOCK] = sprite;
    else
        return 0;

    return 1;
}

int rr_sdl_renderer_load_fonts(rrRenderer* renderer, const char* asset_dir) {
    TTF_Font* font;
    char asset_path[256];
    sprintf(asset_path, "%s%s%s", asset_dir, rr_path_sep(), "ms-sans-serif.ttf");

    font = TTF_OpenFont(asset_path, 24);
    if (font)
        renderer->fonts[RR_FONT_TITLE] = font;
    else
        return 0;

    font = TTF_OpenFont(asset_path, 16);
    if (font)
        renderer->fonts[RR_FONT_BUTTON] = font;
    else
        return 0;


    return 1;
}