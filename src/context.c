#include "Context.h"

struct Context
{
    uint pixel_size;
    uint canvas_size;
    uint row_size;
    uint col_size;
    uint x_offset;
    uint y_offset;
    color_t pixels[SPRITE_CANVAS_SIZE];
    SDL_Rect rects[SPRITE_CANVAS_SIZE];
};

extern Context_T Context_make(uint pixel_size, uint row_size, uint col_size, uint x_offset, uint y_offset)
{
    Context_T ctx = malloc(sizeof(struct Context));
    ;

    ctx->row_size = row_size;
    ctx->col_size = col_size;

    char i, j;
    char index = 0;
    for (i = 0; i < col_size; i++)
    {
        for (j = 0; j < row_size; j++)
        {
            ctx->pixels[index] = BLACK;
            ctx->rects[index].x = x_offset + j * pixel_size;
            ctx->rects[index].y = y_offset + i * pixel_size;
            ctx->rects[index].w = pixel_size;
            ctx->rects[index].h = pixel_size;
            index++;
        }
    }

    return ctx;
}

extern Context_T Context_free(Context_T ctx)
{
    free(ctx);
}

extern void Context_render(Context_T ctx)
{
    char i;

    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    {
        set_pixel_render_color(ctx->pixels[i]);
        SDL_RenderFillRect(renderer, &ctx->rects[i]);
    }
}

extern void Context_swap_pixels(Context_T dest, Context_T source)
{
    memcpy(dest->pixels, source->pixels, sizeof(dest->pixels));
}

extern void Context_focus(Context_T dest, Context_T source)
{
    memcpy(dest->rects, source->rects, sizeof(dest->rects));
}

static int XYInRect(const SDL_Rect rect)
{
    return ((mouse.x >= rect.x && mouse.x <= rect.x + rect.w) && (mouse.y >= rect.y && mouse.y <= rect.y + rect.h));
}

extern void Context_handle_rect_click(Context_T ctx, void (*fn)(const unsigned char))
{
    unsigned char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
        if (XYInRect(ctx->rects[i]))
            (*fn)(i);
}

extern void Context_from_pixel_buffer(Context_T ctx, color_t *pixel_buffer)
{
    memcpy(ctx->pixels, pixel_buffer, sizeof(ctx->pixels));
}

extern void Context_to_pixel_buffer(Context_T ctx, color_t *pixel_buffer)
{
    memcpy(pixel_buffer, ctx->pixels, sizeof(ctx->pixels));
}

extern void Context_swap_rect_buffer(Context_T ctx, SDL_Rect *rect_buffer)
{
    memcpy(ctx->rects, rect_buffer, sizeof(ctx->rects));
}

extern color_t Context_get_pixel(Context_T ctx, const unsigned char pixel_index)
{
    return ctx->pixels[pixel_index];
}

extern void Context_set_pixel(Context_T ctx, const unsigned char pixel_index, color_t color)
{
    ctx->pixels[pixel_index] = color;
}
