#include "sdl_renderer.h"



#include <SDL_image.h>

#include <stdlib.h>



rrRenderer* rr_sdl_renderer_create(SDL_Renderer* sdl_renderer) {
    rrRenderer* renderer = malloc(sizeof(rrRenderer));
    renderer->renderer = sdl_renderer;

    renderer->text_sprites = cutil_vector_create(cutil_trait_ptr());
    renderer->sprites = calloc(RR_SPRITE_COUNT, sizeof(rrSprite*));
    renderer->fonts = calloc(RR_FONT_COUNT, sizeof(TTF_Font*));
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

    free(renderer->sprites);
    free(renderer->fonts);
    cutil_vector_destroy(renderer->text_sprites);
}

void rr_sdl_renderer_set_screen_size(rrRenderer* renderer, rrPoint* screen_size) {
    rr_point_copy(&renderer->screen_size, screen_size);
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
