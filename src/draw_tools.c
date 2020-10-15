#include "globals.h"

static void tool_toolbar_selection(const unsigned int rect_index);

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

static void tool_pen(const unsigned int rect_index)
{
    Context_set_pixel(sprite_canvas_ctx, rect_index, pen_color);
    Context_t current_cell = sprite_selector_cells[current_sprite_index];
    Context_swap_pixels(current_cell, sprite_canvas_ctx);
}

static void tool_alt_pen(const unsigned int rect_index)
{
    pen_color = Context_get_pixel(sprite_canvas_ctx, rect_index);
}

/**
* tool_fill_recurse
* recursive function that fills in neighbors with the current `pen_color` if the conditions are correct
*/
static void tool_fill_recurse(const unsigned int rect_index, color_t original_color)
{
    if (original_color == pen_color) return;
    if (!canvas_index_in_range(rect_index)) return;
    if (Context_get_pixel(sprite_canvas_ctx, rect_index) != original_color) return;

    tool_pen(rect_index);

	increment_batch_operation_count();

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

static void tool_fill(const unsigned int rect_index)
{
    color_t original_color = Context_get_pixel(sprite_canvas_ctx, rect_index);
    tool_fill_recurse(rect_index, original_color);
}

static void tool_sprite_selection(const unsigned int rect_index)
{
    current_sprite_index = rect_index;
    Context_indicator_focus(sprite_selector_ctx, current_sprite_index);
    Context_swap_pixels(sprite_canvas_ctx, sprite_selector_cells[rect_index]);
}

static void tool_color_pick(const unsigned int rect_index)
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
    Context_indicator_focus(toolbar_ctx, PEN);
}

static void undo()
{
    for (int i = 0; i < batch_operation_counter + 1; i++)
	{
		printf("Undo called\n");
		Message_Queue_enqueue(command_message_queue, "undo", 0);
		Context_move_commits(sprite_canvas_ctx, -1);
		Context_t current_cell = sprite_selector_cells[current_sprite_index];
		Context_swap_pixels(current_cell, sprite_canvas_ctx);
	}
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

void Draw_tool_handle_event(draw_event_t event)
{
    switch(event)
    {
        case ACTIVATE_PEN:
            active_tool = PEN;
            Message_Queue_enqueue(command_message_queue, "pen tool", 0);
            Context_indicator_focus(toolbar_ctx, PEN);
            break;
        case ACTIVATE_FILL:
            active_tool = FILL;
            Message_Queue_enqueue(command_message_queue, "fill tool", 0);
            Context_indicator_focus(toolbar_ctx, FILL);
            break;
        case LEFT_CLICK_EVENT:
            left_clicks();
            break;
        case RIGHT_CLICK_EVENT:
            right_clicks();
            break;
        case OPEN_FILE:
            open_file(lshift);
            break;
        case SAVE_FILE:
            save_file(lshift);
            break;
        case PASTE_SPRITE:
            paste_sprite();
            break;
        case COPY_SPRITE:
            copy_sprite();
            break;
        case HANDLE_REDO:
            redo();
            break;
        case HANDLE_UNDO:
            undo();
            Context_indicator_focus(toolbar_ctx, PEN);
            break;
        case LEFT_ARROW:
            decrement_sprite_selector();
            break;
        case RIGHT_ARROW:
            increment_sprite_selector();
            break;
        case UP_ARROW:
            decrement_row_sprite_selector();
            break;
        case DOWN_ARROW:
            increment_row_sprite_selector();
            break;
        case SHOW_HELP:
            help();
        default:
            break;
    }
}

static void tool_toolbar_selection(const unsigned int rect_index)
{
    switch(rect_index)
    {
        case PEN:
            Draw_tool_handle_event(ACTIVATE_PEN);
            break;
        case FILL:
            Draw_tool_handle_event(ACTIVATE_FILL);
            break;
        case DRAG:
            Draw_tool_handle_event(ACTIVATE_DRAG);
            break;
        case UNDO:
            Draw_tool_handle_event(HANDLE_UNDO);
            break;
        case REDO:
            Draw_tool_handle_event(HANDLE_REDO);
            break;
        case LOAD:
            Draw_tool_handle_event(OPEN_FILE);
            break;
        case SAVE:
            Draw_tool_handle_event(SAVE_FILE);
            break;
        case INFO:
            Draw_tool_handle_event(SHOW_HELP);
            break;
        default:
            break;
    }
}
