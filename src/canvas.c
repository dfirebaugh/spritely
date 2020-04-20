#include "canvas.h"

void init_sprite_canvas()
{
    sprite_canvas_ctx.pixel_size = SPRITE_CANVAS_PIXEL_SIZE;
    sprite_canvas_ctx.canvas_size = SPRITE_CANVAS_SIZE;
    sprite_canvas_ctx.row_size = SPRITE_CANVAS_ROW_SIZE;
    sprite_canvas_ctx.x_offset = 0;
    sprite_canvas_ctx.y_offset = 0;
    sprite_canvas_ctx.canvas = (pixel *)sprite_canvas;

    main_color = BLUE;

    init_context(sprite_canvas_ctx);
}

void render_sprite_ctx()
{
    render_context(sprite_canvas_ctx);
}

void canvas_to_spritesheet(const char sprite_sheet_index)
{
    char spr[SPRITE_CANVAS_SIZE];
    char i, j;
    char index = 0;
    for (i = 0; i < SPRITE_CANVAS_ROW_SIZE; i++)
    {
        for (j = 0; j < SPRITE_CANVAS_ROW_SIZE; j++)
        {
            spr[index] = sprite_canvas[i][j].color;
            index++;
        }
    }
    push_to_sprite_sheet(sprite_sheet_index, spr);
}

void sprite_canvas_left_click()
{
    char i, j;
    char index = 0;
    for (i = 0; i < SPRITE_CANVAS_ROW_SIZE; i++)
    {
        for (j = 0; j < SPRITE_CANVAS_ROW_SIZE; j++)
        {
            if (XYInRect(sprite_canvas[i][j].rect))
            {
                sprite_canvas[i][j].color = main_color;
                canvas_to_spritesheet(index);
            }
            index++;
        }
    }
}

void sprite_canvas_right_click()
{
    char i, j;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
    {
        for (j = 0; j < SPRITE_CANVAS_ROW_SIZE; j++)
        {
            if (XYInRect(sprite_canvas[i][j].rect))
                main_color = sprite_canvas[i][j].color;
        }
    }
}
