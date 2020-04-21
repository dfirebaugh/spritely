#include "globals.h"

#ifndef _CONTEXT
#define _CONTEXT

typedef struct context{
    uint pixel_size;
    uint canvas_size;
    uint row_size;
    uint col_size;
    uint x_offset;
    uint y_offset;
    unsigned char pixels[SPRITE_CANVAS_SIZE];
    SDL_Rect rects[SPRITE_CANVAS_SIZE];
} context;

void context_render(context *ctx);
context context_make(uint pixel_size, uint row_size, uint col_size, uint x_offset, uint y_offset);

#endif