
#include "globals.h"
#include "util.h"
#include "context.h"

#ifndef _SPRITE_CANVAS
#define _SPRITE_CANVAS

context sprite_canvas_ctx;

/* an 8x8 grid for drawing a sprite */
pixel sprite_canvas[SPRITE_CANVAS_SIZE];

void init_sprite_canvas();
void render_sprite_ctx();

void sprite_canvas_left_click();
void sprite_canvas_right_click();

#endif
