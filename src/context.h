#include "globals.h"
#include "util.h"

#ifndef _CONTEXT
#define _CONTEXT

typedef struct context{
    int pixel_size;
    int canvas_size;
    int row_size;
    int x_offset;
    int y_offset;
    pixel *canvas;
} context;

// context make_context(pixel canvas, int canvas_size, int pixel_size, int row_size, int x_offset, int y_offset);
void init_context(context ctx);
void render_context(context ctx);

#endif