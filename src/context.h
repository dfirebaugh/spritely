#include "globals.h"
#include "util.h"

#ifndef _CONTEXT
#define _CONTEXT
typedef struct context{
    uint pixel_size;
    uint canvas_size;
    uint row_size;
    uint col_size;
    uint x_offset;
    uint y_offset;
    color_t pixels[SPRITE_CANVAS_SIZE];
    SDL_Rect rects[SPRITE_CANVAS_SIZE];
} context;

extern void context_render(context *ctx);
extern context context_make(uint pixel_size, uint row_size, uint col_size, uint x_offset, uint y_offset);

#endif