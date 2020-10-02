#include "spritesheet.h"

#include <stdlib.h>
#include <string.h>

rrSpritesheet* rr_spritesheet_create(rrSprite* sprite, size_t size){
    rrSpritesheet* spritesheet = malloc(sizeof(rrSpritesheet));
    spritesheet->source = sprite;
    spritesheet->sprites = calloc(size, sizeof(rrSprite*));
    spritesheet->size = size;

    return spritesheet;
}

void rr_spritesheet_destroy(rrSpritesheet* spritesheet){
    size_t i;

    for (i = 0; i < spritesheet->size; i++) {
        if (spritesheet->sprites[i])
            free(spritesheet->sprites[i]);
    }

    free(spritesheet);
}

rrSprite* rr_spritesheet_add_sprite(rrSpritesheet* spritesheet, size_t index, rrRect* rect) {
    rrSprite* sprite = malloc(sizeof(rrSprite));

    if (spritesheet->sprites[index])
        free(spritesheet->sprites[index]);

    sprite->handle = spritesheet->source->handle;
    memcpy(&sprite->rect, rect, sizeof(rrRect));
    spritesheet->sprites[index] = sprite;

    return sprite;
}
