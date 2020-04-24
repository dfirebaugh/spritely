#include "globals.h"
#include "file.h"

static void tool_pen(const unsigned char rect_index)
{
    Context_set_pixel(sprite_canvas_ctx, rect_index, pen_color);
    Context_t current_cell = sprite_selector_cells[current_sprite_index];
    Context_swap_pixels(current_cell, sprite_canvas_ctx);
}

static void tool_alt_pen(const unsigned char rect_index)
{
    pen_color = Context_get_pixel(sprite_canvas_ctx, rect_index);
}

static void tool_sprite_selection(const unsigned char rect_index)
{
    current_sprite_index = rect_index;
    Context_focus(sprite_selection_indicator, sprite_selector_cells[rect_index]);
    Context_swap_pixels(sprite_canvas_ctx, sprite_selector_cells[rect_index]);
}

static void tool_color_pick(const unsigned char rect_index)
{
    pen_color = rect_index;
}

static void left_clicks()
{
    Context_handle_rect_click(sprite_canvas_ctx, tool_pen);
    Context_handle_rect_click(color_picker_ctx, tool_color_pick);
    Context_handle_rect_click(sprite_selector_ctx, tool_sprite_selection);
}

static void right_clicks()
{
    Context_handle_rect_click(sprite_canvas_ctx, tool_alt_pen);
    Context_handle_rect_click(color_picker_ctx, tool_color_pick);
    Context_handle_rect_click(sprite_selector_ctx, tool_sprite_selection);
}

static void copy_sprite()
{
    Message_Queue_enqueue(command_message_queue, "copied");
    Context_to_pixel_buffer(sprite_canvas_ctx, clipboard_pixel_buffer);
    Context_swap_pixels(sprite_selector_cells[current_sprite_index], sprite_canvas_ctx);
}

static void paste_sprite()
{
    Message_Queue_enqueue(command_message_queue, "paste");
    Context_from_pixel_buffer(sprite_canvas_ctx, clipboard_pixel_buffer);
    Context_swap_pixels(sprite_selector_cells[current_sprite_index], sprite_canvas_ctx);
}

static void free_all_contexts()
{

    Context_free(sprite_canvas_ctx);
    Context_free(sprite_selector_ctx);
    Context_free(color_picker_ctx);

    char i, j;
    char index = 0;
    for (i = 0; i < SPRITESHEET_COL_SIZE; i++)
    {
        for (j = 0; j < SPRITESHEET_ROW_SIZE; j++)
        {
            Context_free(sprite_selector_cells[index]);
            index++;
        }
    }
}

void process_inputs()
{
    SDL_Event event;

    /* Loop through waiting messages and process them */
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        /* Closing the Window or pressing Escape will exit the program */
        case SDL_QUIT:
            free_all_contexts();
            Message_Queue_free(command_message_queue);
            exit(0);
            break;
        case SDL_MOUSEMOTION:
            mouse.x = event.motion.x;
            mouse.y = event.motion.y;
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
                Message_Queue_free(command_message_queue);
                free_all_contexts();
                exit(0);
                break;

            case SDLK_s:
            case SDLK_DOWN:
                if (lctrl)
                    // save_file();
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
                    copy_sprite();
                break;
            case SDLK_v:
                if (lctrl)
                    paste_sprite();
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
