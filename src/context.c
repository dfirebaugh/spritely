#include "context.h"

void init_context(context ctx, pixel *canvas)
{
    char i;
    char col = 0;
    char row = 0;
    for (i = 0; i < ctx.canvas_size; i++)
    {
        col++;

        if (i % ctx.row_size == 0)
        {
            if (i == 0)
            {
                canvas[i].rect.y = 0;
            }
            else
            {
                row++;
            }
            col = 0;
        }

        canvas[i].rect.x = ctx.x_offset + col * ctx.pixel_size;
        canvas[i].rect.y = ctx.y_offset + row * ctx.pixel_size;
        canvas[i].rect.w = ctx.pixel_size;
        canvas[i].rect.h = ctx.pixel_size;
    }
}

void render_context(context ctx, pixel *canvas)
{
    char i;
    for (i = 0; i < ctx.canvas_size; i++)
    {
        pixel p = canvas[i];

        set_pixel_render_color(p);

        SDL_RenderFillRect(renderer, &p.rect);
    }
}
