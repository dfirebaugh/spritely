#include "globals.h"
#include "context.h"

context color_picker_ctx;

pixel color_picker_canvas[COLORPICKER_ROW_SIZE][COLORPICKER_ROW_SIZE];

void init_color_picker();
void render_color_picker();
void color_picker_click();
