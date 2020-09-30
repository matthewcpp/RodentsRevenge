#ifndef RR_SPRITESHEET_INDEXSHEET_H
#define RR_SPRITESHEET_INDEXSHEET_H

#include "rect.h"
#include "sprite.h"

typedef struct {
    rrSprite* source;
    rrSprite** sprites;
    size_t size;
} rrSpritesheet;

rrSpritesheet* rr_spritesheet_create(rrSprite* sprite, size_t size);
void rr_spritesheet_destroy(rrSpritesheet* spritesheet);

rrSprite* rr_spritesheet_add_sprite(rrSpritesheet* spritesheet, size_t index, rrRect* rect);

#endif
