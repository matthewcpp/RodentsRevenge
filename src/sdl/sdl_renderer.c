#include "sdl_renderer.h"

#include "cutil/vector.h"

#include <stdlib.h>

struct rrRenderer{
    SDL_Renderer* renderer;
    rrPoint screen_size;

    TTF_Font* font;
    cutil_vector* text_sprites;
};

rrRenderer* rr_sdl_renderer_create(SDL_Renderer* sdl_renderer) {
    rrRenderer* renderer = malloc(sizeof(rrRenderer));
    renderer->renderer = sdl_renderer;

    renderer->text_sprites = cutil_vector_create(cutil_trait_ptr());

    return renderer;
}

void rr_sdl_renderer_destroy(rrRenderer* renderer) {
    (void)renderer;
}

void rr_sdl_renderer_set_font(rrRenderer* renderer, TTF_Font* font) {
    renderer->font = font;
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

rrSprite* rr_renderer_create_text(struct rrRenderer* renderer, int font, const char* str) {
    rrSprite* textSprite = malloc(sizeof(rrSprite));
    SDL_Surface* text_surface = NULL;
    SDL_Color color={255,255,255, 255};

    text_surface = TTF_RenderUTF8_Solid (renderer->font, str, color);
    textSprite->handle = SDL_CreateTextureFromSurface(renderer->renderer, text_surface);
    SDL_FreeSurface(text_surface);
    SDL_QueryTexture(textSprite->handle, NULL, NULL, &textSprite->rect.w, &textSprite->rect.h);
    textSprite->rect.x = 0;
    textSprite->rect.y = 0;

    cutil_vector_push_back(renderer->text_sprites, &textSprite);

    (void)font;

    return textSprite;
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

void rr_color_black(rrColor* color) {
    color->r = 0;
    color->g = 0;
    color->b = 0;
    color->a = 255;
}

void rr_color_white(rrColor* color) {
    color->r = 255;
    color->g = 255;
    color->b = 255;
    color->a = 255;
}