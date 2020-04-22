#include "context.h"

extern context context_make(uint pixel_size, uint row_size, uint col_size, uint x_offset, uint y_offset)
{
    context ctx;

    ctx.row_size = row_size;
    ctx.col_size = col_size;

    char i, j;
    char index = 0;
    for (i = 0; i < col_size; i++)
    {
        for (j = 0; j < row_size; j++)
        {
            ctx.pixels[index] = BLACK;
            ctx.rects[index].x = x_offset + j * pixel_size;
            ctx.rects[index].y = y_offset + i * pixel_size;
            ctx.rects[index].w = pixel_size;
            ctx.rects[index].h = pixel_size;
            index++;
        }
    }

    return ctx;
}

extern void context_render(context *ctx)
{
    char i;

    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    {
        set_pixel_render_color(ctx->pixels[i]);
        SDL_RenderFillRect(renderer, &ctx->rects[i]);
    }
}

extern void context_swap_pixels(context *dest, context *source)
{
    memcpy(dest->pixels, source->pixels, sizeof(dest->pixels));
}

extern void context_focus(context *dest, context *source)
{
    memcpy(dest->rects, source->rects, sizeof(dest->rects));
}

static int XYInRect(const SDL_Rect rect)
{
    return ((mouse.x >= rect.x && mouse.x <= rect.x + rect.w) && (mouse.y >= rect.y && mouse.y <= rect.y + rect.h));
}

extern void context_handle_rect_click(const context ctx, void (*fn)(const unsigned char))
{
    unsigned char i;
    for (i = 0; i < (ctx.row_size * ctx.col_size); i++)
        if (XYInRect(ctx.rects[i]))
            (*fn)(i);
}