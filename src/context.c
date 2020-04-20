#include "context.h"

void init_context(context ctx)
{
    char i, j;

    for (i = 0; i < ctx.row_size; i++)
    {
        for (j = 0; j < ctx.row_size; j++)
        {
            ctx.canvas[j * ctx.row_size + i].rect.x = ctx.x_offset + i * ctx.pixel_size;
            ctx.canvas[j * ctx.row_size + i].rect.y = ctx.y_offset + j * ctx.pixel_size;
            ctx.canvas[j * ctx.row_size + i].rect.w = ctx.pixel_size;
            ctx.canvas[j * ctx.row_size + i].rect.h = ctx.pixel_size;
        }
    }
}

void render_context(context ctx)
{
    char i, j;
    for (i = 0; i < ctx.row_size; i++)
    {

        for (j = 0; j < ctx.row_size; j++)
        {
            set_pixel_render_color(ctx.canvas[j * ctx.row_size + i]);
            SDL_RenderFillRect(renderer, &ctx.canvas[j * ctx.row_size + i].rect);
        }
    }
}
