#include "color_picker.h"

void init_color_picker()
{
    color_picker_ctx.pixel_size = COLORPICKER_PIXEL_SIZE;
    color_picker_ctx.canvas_size = COLORPICKER_CANVAS_SIZE;
    color_picker_ctx.row_size = COLORPICKER_ROW_SIZE;
    color_picker_ctx.x_offset = COLORPICKER_XOFFSET;
    color_picker_ctx.y_offset = 0;
    color_picker_ctx.canvas = (pixel *)color_picker_canvas;

    char i, j;
    char index = 0;
    for (i = 0; i < COLORPICKER_ROW_SIZE; i++)
    {
        for (j = 0; j < COLORPICKER_ROW_SIZE; j++)
        {
            color_picker_canvas[i][j].color = index;
            index++;
        }
    }

    init_context(color_picker_ctx);
}

void render_color_picker()
{
    render_context(color_picker_ctx);
}

void color_picker_click()
{
    char i, j;
    char index = 0;
    for (i = 0; i < COLORPICKER_ROW_SIZE; i++)
    {
        for (j = 0; j < COLORPICKER_ROW_SIZE; j++)
        {
            if (XYInRect(color_picker_canvas[i][j].rect))
                main_color = index;
        index++;
        }
    }
}
