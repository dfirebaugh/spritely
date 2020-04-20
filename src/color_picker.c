#include "color_picker.h"

void init_color_picker()
{
    color_picker_ctx.pixel_size = COLORPICKER_PIXEL_SIZE;
    color_picker_ctx.canvas_size = COLORPICKER_CANVAS_SIZE;
    color_picker_ctx.row_size = COLORPICKER_ROW_SIZE;
    color_picker_ctx.x_offset = COLORPICKER_XOFFSET;
    color_picker_ctx.y_offset = 0;

    int i;

    for (i = 0; i < color_picker_ctx.canvas_size; i++)
    {
        color_picker_canvas[i].color = i;
    }

    init_context(color_picker_ctx, color_picker_canvas);
}

void render_color_picker()
{
    render_context(color_picker_ctx, color_picker_canvas);
}

void color_picker_click()
{
    int i;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
    {
        if(XYInRect(color_picker_canvas[i].rect))
            main_color = i;
    }
}
