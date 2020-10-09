#include "globals.h"
#include "util.h"
#include "file.h"

static void help()
{
    Message_Queue_enqueue(help_message_queue,
        "> Ctrl+C - Copy\n"
        "> Ctrl+V - Paste\n"
        "> Ctrl+Z - Undo\n"
        "> Ctrl+Shift+Z | Ctrl+Y - Redo\n"
        "> Ctrl+S - Save the spritesheet\n"
        "> Ctrl+Shift+S - Save the spritesheet and images for each sprite\n"
        "> Ctrl+O - Load a spritesheet from an image file\n"
        "> Left click to draw pixel\n"
        "> Right click to select a colour that is on the  canvas\n"
        "> Arrow Keys to move sprite selection"
        "> F - fill tool"
        "> Space - pen tool",
        1
    );
}

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

/**
* tool_fill_recurse
* recursive function that fills in neighbors with the current `pen_color` if the conditions are correct
*/
static void tool_fill_recurse(const unsigned char rect_index, color_t original_color)
{
    if (original_color == pen_color) return;
    if (!canvas_index_in_range(rect_index)) return;
    if (Context_get_pixel(sprite_canvas_ctx, rect_index) != original_color) return;

    tool_pen(rect_index);

    if (canvas_index_in_range(rect_index - 1))
    {
        if (!(rect_index % SPRITE_CANVAS_ROW_SIZE == 0))
        {
            tool_fill_recurse(rect_index - 1, original_color);
        }
    }

    if (canvas_index_in_range(rect_index + 1))
    {
        if (!(rect_index % SPRITE_CANVAS_ROW_SIZE == 7))
        {
            tool_fill_recurse(rect_index + 1, original_color);
        }
    }

    if (canvas_index_in_range(rect_index + SPRITE_CANVAS_ROW_SIZE))
    {
        if (!(rect_index + SPRITE_CANVAS_ROW_SIZE % SPRITE_CANVAS_ROW_SIZE == 7))
        {
            tool_fill_recurse(rect_index + SPRITE_CANVAS_ROW_SIZE, original_color);
        }
    }

    if (canvas_index_in_range(rect_index - SPRITE_CANVAS_ROW_SIZE))
    {
        if (!(rect_index - SPRITE_CANVAS_ROW_SIZE % SPRITE_CANVAS_ROW_SIZE == 7))
        {
            tool_fill_recurse(rect_index - SPRITE_CANVAS_ROW_SIZE, original_color);
        }
    }
}

static void tool_fill(const unsigned char rect_index)
{
    color_t original_color = Context_get_pixel(sprite_canvas_ctx, rect_index);
    tool_fill_recurse(rect_index, original_color);
}

static void tool_sprite_selection(const unsigned char rect_index)
{
    current_sprite_index = rect_index;
    Context_indicator_focus(sprite_selector_ctx, current_sprite_index);
    Context_swap_pixels(sprite_canvas_ctx, sprite_selector_cells[rect_index]);
}

static void tool_toolbar_selection(const unsigned char rect_index)
{
/**
 * this is where we can handle which tool is selected based on 
 * which thing on the toolbar is active
 */
}

static void tool_color_pick(const unsigned char rect_index)
{
    pen_color = rect_index;
}

static void left_clicks()
{
    if (active_tool == FILL) {
        Context_handle_rect_click(sprite_canvas_ctx, tool_fill);
    } else {
        Context_handle_rect_click(sprite_canvas_ctx, tool_pen);
    }

    Context_handle_rect_click(color_picker_ctx, tool_color_pick);
    Context_handle_rect_click(sprite_selector_ctx, tool_sprite_selection);
    Context_handle_rect_click(toolbar_ctx, tool_toolbar_selection);
}

static void right_clicks()
{
    Context_handle_rect_click(sprite_canvas_ctx, tool_alt_pen);
    Context_handle_rect_click(color_picker_ctx, tool_color_pick);
    Context_handle_rect_click(sprite_selector_ctx, tool_sprite_selection);
}

static void copy_sprite()
{
    Message_Queue_enqueue(command_message_queue, "copied", 0);
    Context_to_pixel_buffer(sprite_canvas_ctx, clipboard_pixel_buffer);
    Context_swap_pixels(sprite_selector_cells[current_sprite_index], sprite_canvas_ctx);
}

static void paste_sprite()
{
    Message_Queue_enqueue(command_message_queue, "paste", 0);
    Context_from_pixel_buffer(sprite_canvas_ctx, clipboard_pixel_buffer);
    Context_swap_pixels(sprite_selector_cells[current_sprite_index], sprite_canvas_ctx);
}

static void redo()
{
    Message_Queue_enqueue(command_message_queue, "redo", 0);
    Context_move_commits(sprite_canvas_ctx, 1);
    Context_t current_cell = sprite_selector_cells[current_sprite_index];
    Context_swap_pixels(current_cell, sprite_canvas_ctx);
}

static void undo()
{
    Message_Queue_enqueue(command_message_queue, "undo", 0);
    Context_move_commits(sprite_canvas_ctx, -1);
    Context_t current_cell = sprite_selector_cells[current_sprite_index];
    Context_swap_pixels(current_cell, sprite_canvas_ctx);
}

static void increment_sprite_selector()
{
    if (!(sprite_sheet_index_in_range(current_sprite_index + 1))) return;

    current_sprite_index++;
    tool_sprite_selection(current_sprite_index);
}

static void decrement_sprite_selector()
{
    if (!(sprite_sheet_index_in_range(current_sprite_index - 1))) return;

    current_sprite_index--;
    tool_sprite_selection(current_sprite_index);
}

static void increment_row_sprite_selector()
{
    if (!(sprite_sheet_index_in_range(current_sprite_index + SPRITESHEET_ROW_SIZE))) return;

    current_sprite_index += SPRITESHEET_ROW_SIZE;
    tool_sprite_selection(current_sprite_index);
}

static void decrement_row_sprite_selector()
{
    if (!(sprite_sheet_index_in_range(current_sprite_index - SPRITESHEET_ROW_SIZE))) return;

    current_sprite_index -= SPRITESHEET_ROW_SIZE;
    tool_sprite_selection(current_sprite_index);
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
            Message_Queue_free(help_message_queue);
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
                Message_Queue_free(help_message_queue);
                free_all_contexts();
                exit(0);
                break;
            case SDLK_s:
                if (lctrl) {
                    save_file(lshift);
                }
                break;
            case SDLK_o:
                if (lctrl) {
                    open_file();
                }
                break;
            case SDLK_F1:
                help();
                break;
            case SDLK_LCTRL:
                lctrl = 1;
                break;
            case SDLK_LSHIFT:
                lshift = 1;
                break;
            case SDLK_c:
                if (lctrl)
                    copy_sprite();
                break;
            case SDLK_v:
                if (lctrl)
                    paste_sprite();
                break;
            case SDLK_y:
                if (lctrl)
                    redo();
                break;
            case SDLK_z:
                if (lctrl && !lshift)
                    undo();
                else if (lctrl && lshift)
                    redo();
            case SDLK_f:
                active_tool = FILL;
                Message_Queue_enqueue(command_message_queue, "fill tool", 0);
                break;
            case SDLK_SPACE:
                active_tool = PEN;
                Message_Queue_enqueue(command_message_queue, "pen tool", 0);
                break;
            case SDLK_LEFT:
                decrement_sprite_selector();
                break;
            case SDLK_RIGHT:
                increment_sprite_selector();
                break;
            case SDLK_DOWN:
                increment_row_sprite_selector();
                break;
            case SDLK_UP:
                decrement_row_sprite_selector();
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
            case SDLK_LSHIFT:
                lshift = 0;
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
