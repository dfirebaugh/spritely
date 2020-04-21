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
    pixel pixels[64];
} context;

void context_render(context *ctx);
context context_make(uint pixel_size, uint row_size, uint col_size, uint x_offset, uint y_offset);

#endif