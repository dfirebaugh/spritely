#include "canvas.h"

void set_pixel_render_color(pixel p);

void init_sprite_canvas()
{
    char i;
    char col = 0;
    char row = 0;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
    {
        col++;

        if (i % 8 == 0)
        {
            if (i == 0)
            {
                sprite_canvas[i].rect.y = 0;
            }
            else
            {
                row++;
            }
            col = 0;
        }

        sprite_canvas[i].rect.x = col * SPRITE_CANVAS_PIXEL_SIZE;
        sprite_canvas[i].rect.w = SPRITE_CANVAS_PIXEL_SIZE;
        sprite_canvas[i].rect.h = SPRITE_CANVAS_PIXEL_SIZE;
        sprite_canvas[i].rect.y = row * SPRITE_CANVAS_PIXEL_SIZE;
    }
}

void render_sprite_canvas()
{
    char i;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
    {
        pixel p = sprite_canvas[i];

        set_pixel_render_color(p);

        SDL_RenderFillRect(renderer, &p.rect);
    }
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