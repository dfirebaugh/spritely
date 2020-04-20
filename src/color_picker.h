#include "globals.h"
#include "util.h"

/* an 8x8 grid for drawing a sprite */
pixel color_picker_canvas[SPRITE_CANVAS_SIZE];

void init_color_picker();
void render_color_picker();
void color_picker_click();
