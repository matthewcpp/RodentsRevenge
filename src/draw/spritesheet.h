#ifndef RR_SPRITESHEET_INDEXSHEET_H
#define RR_SPRITESHEET_INDEXSHEET_H

#include "../draw/renderer.h"

#include "cutil/btree.h"

typedef struct {
    rrSprite* source;
    rrSprite** sprites;
    size_t size;
} rrSpritesheet;

rrSpritesheet* rr_spritesheet_create(rrSprite* sprite, size_t size);
void rr_spritesheet_init(rrSpritesheet* spritesheet, rrSprite* sprite, size_t size);
rrSprite* rr_spritesheet_add_sprite(rrSpritesheet* spritesheet, size_t index, rrRect* rect);
void rr_spritesheet_destroy(rrSpritesheet* spritesheet);

#endif
