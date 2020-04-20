
#include "globals.h"
#include "sprite_sheet.h"

#ifndef _SPRITE_CANVAS
#define _SPRITE_CANVAS

context sprite_canvas_ctx;

/* an 8x8 grid for drawing a sprite */
pixel sprite_canvas[SPRITE_CANVAS_ROW_SIZE][SPRITE_CANVAS_ROW_SIZE];

void init_sprite_canvas();
void render_sprite_ctx();

void sprite_canvas_left_click();
void sprite_canvas_right_click();

#endif
