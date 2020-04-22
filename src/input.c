#include "input.h"

static void pen(const unsigned char rect_index)
{
    context *current_cell = sprite_selector_cells + current_sprite;
    sprite_canvas_ctx.pixels[rect_index] = pen_color;
    context_swap_pixels(current_cell, &sprite_canvas_ctx);
}

static void alt_pen(const unsigned char rect_index)
{
    pen_color = sprite_canvas_ctx.pixels[rect_index];
}

static void sprite_selection(const unsigned char rect_index)
{
    current_sprite = rect_index;
    context_swap_pixels(&sprite_canvas_ctx, &sprite_selector_cells[rect_index]);
    context_focus(&sprite_selection_indicator, &sprite_selector_cells[rect_index]);
}

static void color_pick(const unsigned char rect_index)
{
    pen_color = rect_index;
}

static void left_clicks()
{
    context_handle_rect_click(sprite_canvas_ctx, pen);
    context_handle_rect_click(sprite_selector_ctx, sprite_selection);
    context_handle_rect_click(color_picker_ctx, color_pick);
}

static void right_clicks()
{
    context_handle_rect_click(sprite_canvas_ctx, alt_pen);
    context_handle_rect_click(sprite_selector_ctx, sprite_selection);
    context_handle_rect_click(color_picker_ctx, color_pick);
}

static void buffer_copy_from_canvas()
{
    memcpy(pixel_buffer, sprite_canvas_ctx.pixels, sizeof(sprite_canvas_ctx.pixels));
}

static void buffer_copy_to_canvas()
{
    memcpy(sprite_canvas_ctx.pixels, pixel_buffer, sizeof(sprite_canvas_ctx.pixels));
    memcpy(sprite_selector_cells[current_sprite].pixels, pixel_buffer, sizeof(sprite_canvas_ctx.pixels));
}

extern void process_inputs()
{
    SDL_Event event;

    /* Loop through waiting messages and process them */
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        /* Closing the Window or pressing Escape will exit the program */
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_MOUSEMOTION:
            mouse.x = event.motion.x;
            mouse.y = event.motion.y;
            // printf("%d == %d\n", event.button.button, SDL_BUTTON_RIGHT);
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                left_clicks();
                break;
            case SDL_BUTTON_RIGHT:
                right_clicks();
                break;
            case SDL_BUTTON_X1:
                /* for some reason right mouse clicks are registering as SDL_BUTTON_X1 but only when I am moving the mouse.... ??????*/
                right_clicks();
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                left_clicks();
                break;
            case SDL_BUTTON_RIGHT:
                right_clicks();
                break;
            default:
                break;
            }
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                exit(0);
                break;

            case SDLK_s:
            case SDLK_DOWN:
                if (lctrl)
                    save_file();
                break;

            case SDLK_d:
            case SDLK_RIGHT:
                break;

            case SDLK_a:
            case SDLK_LEFT:
                break;

            case SDLK_w:
            case SDLK_UP:
                break;

            case SDLK_r:
                break;
            case SDLK_LCTRL:
                lctrl = 1;
                break;
            case SDLK_c:
                if (lctrl)
                    printf("copy\n");
                buffer_copy_from_canvas();
                break;
            case SDLK_v:
                if (lctrl)
                    printf("paste\n");
                buffer_copy_to_canvas();
                break;
            case SDLK_SPACE:
                break;

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_LCTRL:
                lctrl = 0;
                break;

            default:
                break;
            }

        case SDL_USEREVENT:
            break;

        default:
            break;
        }
    }
}
