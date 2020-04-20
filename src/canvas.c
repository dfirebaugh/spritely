#include "canvas.h"

void init_sprite_canvas()
{
    sprite_canvas_ctx.pixel_size = SPRITE_CANVAS_PIXEL_SIZE;
    sprite_canvas_ctx.canvas_size = SPRITE_CANVAS_SIZE;
    sprite_canvas_ctx.row_size = SPRIT_CANVAS_ROW_SIZE;
    sprite_canvas_ctx.x_offset = 0;
    sprite_canvas_ctx.y_offset = 0;

    init_context(sprite_canvas_ctx, sprite_canvas);
}

void render_sprite_ctx()
{
    render_context(sprite_canvas_ctx, sprite_canvas);
}

void sprite_canvas_left_click()
{
    int i;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
    {
        if(XYInRect(sprite_canvas[i].rect))
            sprite_canvas[i].color = main_color;
    }
}

void sprite_canvas_right_click()
{
    int i;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
    {
        if(XYInRect(sprite_canvas[i].rect))
            main_color = sprite_canvas[i].color;
    }
}
