
#include "globals.h"
#include "util.h"

#ifndef _SPRITE_CANVAS
#define _SPRITE_CANVAS

/* an 8x8 grid for drawing a sprite */
pixel sprite_canvas[SPRITE_CANVAS_SIZE];

void init_sprite_canvas();

/* renders the sprite_canvas to the screen */
void render_sprite_canvas();

void sprite_canvas_left_click();
void sprite_canvas_right_click();

#endif
