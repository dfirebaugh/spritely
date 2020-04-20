#include "color_picker.h"

void init_color_picker()
{
    char i;
    char col = 0;
    char row = 0;
    for (i = 0; i < COLORPICKER_CANVAS_SIZE; i++)
    {
        col++;

        if (i % 4 == 0)
        {
            if (i == 0)
            {
                color_picker_canvas[i].rect.y = 0;
            }
            else
            {
                row++;
            }
            col = 0;
        }

        color_picker_canvas[i].color = i;
        color_picker_canvas[i].rect.x = COLORPICKER_OFFSET + col * COLORPICKER_PIXEL_SIZE;
        color_picker_canvas[i].rect.w = COLORPICKER_PIXEL_SIZE;
        color_picker_canvas[i].rect.h = COLORPICKER_PIXEL_SIZE;
        color_picker_canvas[i].rect.y = row * COLORPICKER_PIXEL_SIZE;
    }
}

void render_color_picker()
{
    char i;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
    {
        pixel p = color_picker_canvas[i];

        set_pixel_render_color(p);
        SDL_RenderFillRect(renderer, &p.rect);
    }
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
