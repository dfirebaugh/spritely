#include "globals.h"
#include "util.h"

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

Context_t Context_make(uint pixel_size, uint row_size, uint col_size, uint x_offset, uint y_offset)
{
    Context_t ctx = malloc(sizeof(struct Context));
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

Context_t Context_free(Context_t ctx)
{
    free(ctx);
}

void Context_render(Context_t ctx)
{
    char i;

    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    {
        set_pixel_render_color(ctx->pixels[i]);
        SDL_RenderFillRect(renderer, &ctx->rects[i]);
    }
}

void Context_swap_pixels(Context_t dest, Context_t source)
{
    memcpy(dest->pixels, source->pixels, sizeof(dest->pixels));
}

void Context_indicator_focus(SDL_Rect *indicator, Context_t ctx, const unsigned char rect_index)
{
    memcpy(indicator, &ctx->rects[rect_index], sizeof(SDL_Rect));
}

static int XYInRect(const SDL_Rect rect)
{
    return ((mouse.x >= rect.x && mouse.x <= rect.x + rect.w) && (mouse.y >= rect.y && mouse.y <= rect.y + rect.h));
}

void Context_handle_rect_click(Context_t ctx, void (*fn)(const unsigned char))
{
    unsigned char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
        if (XYInRect(ctx->rects[i]))
            (*fn)(i);
}

void Context_from_pixel_buffer(Context_t ctx, color_t *pixel_buffer)
{
    memcpy(ctx->pixels, pixel_buffer, sizeof(ctx->pixels));
}

void Context_to_pixel_buffer(Context_t ctx, color_t *pixel_buffer)
{
    memcpy(pixel_buffer, ctx->pixels, sizeof(ctx->pixels));
}

void Context_swap_rect_buffer(Context_t ctx, SDL_Rect *rect_buffer)
{
    memcpy(ctx->rects, rect_buffer, sizeof(ctx->rects));
}

color_t Context_get_pixel(Context_t ctx, const unsigned char pixel_index)
{
    return ctx->pixels[pixel_index];
}

void Context_set_pixel(Context_t ctx, const unsigned char pixel_index, color_t color)
{
    ctx->pixels[pixel_index] = color;
}
