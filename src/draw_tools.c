#include "globals.h"

static void tool_toolbar_selection(const unsigned int rect_index);

void show_help()
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
        "> Arrow Keys to move sprite selection\n"
        "> F - fill tool\n"
        "> Space - pen tool\n",
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

bool pixels_to_fill[SPRITE_CANVAS_SIZE];

static void mark_pixel_to_fill(uint8_t pixel_index)
{
    pixels_to_fill[pixel_index] = true;
}

static void clear_pixels_to_fill()
{
    for(uint8_t i = 0; i < SPRITE_CANVAS_SIZE; i++)
    {
        pixels_to_fill[i] = false;
    }
}

/**
* tool_fill_recurse
* recursive function that fills in neighbors with the current `pen_color` if the conditions are correct
*/
static void tool_fill_recurse(const unsigned int rect_index, color_t original_color)
{
    if (pixels_to_fill[rect_index]) return;
    if (!canvas_index_in_range(rect_index)) return;
    if (Context_get_pixel(sprite_canvas_ctx, rect_index) != original_color) return;

    mark_pixel_to_fill(rect_index);

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
    clear_pixels_to_fill();
    color_t original_color = Context_get_pixel(sprite_canvas_ctx, rect_index);
    tool_fill_recurse(rect_index, original_color);
    Context_set_pixels(sprite_canvas_ctx, pixels_to_fill, pen_color);
}

static uint8_t calculate_upshift(uint8_t index)
{
    if (index - SPRITE_CANVAS_ROW_SIZE < 0)
        return SPRITE_CANVAS_SIZE - (SPRITE_CANVAS_ROW_SIZE - index);

    return index - SPRITE_CANVAS_ROW_SIZE;
}

static uint8_t calculate_downshift(uint8_t index)
{
    if (index + SPRITE_CANVAS_ROW_SIZE >= SPRITE_CANVAS_SIZE)
        return SPRITE_CANVAS_ROW_SIZE - (SPRITE_CANVAS_SIZE - index);

    return index + SPRITE_CANVAS_ROW_SIZE;
}

static uint8_t calculate_leftshift(uint8_t index)
{
    if (index % SPRITE_CANVAS_ROW_SIZE == 0)
        return index + SPRITE_CANVAS_ROW_SIZE - 1;

    return index - 1;
}

static uint8_t calculate_rightshift(uint8_t index)
{
    if ((index + 1) % SPRITE_CANVAS_ROW_SIZE == 0)
        return index - SPRITE_CANVAS_ROW_SIZE + 1;

    return index + 1;
}

static void shift_up()
{
    color_t original_pixel_buffer[SPRITE_CANVAS_SIZE];
    color_t new_pixel_buffer[SPRITE_CANVAS_SIZE];
    Context_to_pixel_buffer(sprite_canvas_ctx, original_pixel_buffer);

    for(uint8_t i = 0; i < SPRITE_CANVAS_SIZE; i++)
        new_pixel_buffer[calculate_upshift(i)] = original_pixel_buffer[i];

    Context_from_pixel_buffer(sprite_canvas_ctx, new_pixel_buffer);
}
static void shift_down()
{
    color_t original_pixel_buffer[SPRITE_CANVAS_SIZE];
    color_t new_pixel_buffer[SPRITE_CANVAS_SIZE];
    Context_to_pixel_buffer(sprite_canvas_ctx, original_pixel_buffer);

    for(uint8_t i = 0; i < SPRITE_CANVAS_SIZE; i++)
        new_pixel_buffer[calculate_downshift(i)] = original_pixel_buffer[i];

    Context_from_pixel_buffer(sprite_canvas_ctx, new_pixel_buffer);
}

static void shift_left()
{
    color_t original_pixel_buffer[SPRITE_CANVAS_SIZE];
    color_t new_pixel_buffer[SPRITE_CANVAS_SIZE];
    Context_to_pixel_buffer(sprite_canvas_ctx, original_pixel_buffer);

    for(uint8_t i = 0; i < SPRITE_CANVAS_SIZE; i++)
        new_pixel_buffer[calculate_leftshift(i)] = original_pixel_buffer[i];

    Context_from_pixel_buffer(sprite_canvas_ctx, new_pixel_buffer);
}

static void shift_right()
{
    color_t original_pixel_buffer[SPRITE_CANVAS_SIZE];
    color_t new_pixel_buffer[SPRITE_CANVAS_SIZE];
    Context_to_pixel_buffer(sprite_canvas_ctx, original_pixel_buffer);

    for(uint8_t i = 0; i < SPRITE_CANVAS_SIZE; i++)
        new_pixel_buffer[calculate_rightshift(i)] = original_pixel_buffer[i];

    Context_from_pixel_buffer(sprite_canvas_ctx, new_pixel_buffer);
}

unsigned int previous_rect_index;
static void tool_drag(const unsigned int rect_index)
{
    if(previous_rect_index - SPRITE_CANVAS_ROW_SIZE == rect_index)
        shift_up();

    if(previous_rect_index + SPRITE_CANVAS_ROW_SIZE == rect_index)
        shift_down();
    
    if(previous_rect_index - 1 == rect_index)
        shift_left();

    if(previous_rect_index + 1 == rect_index)
        shift_right();


    Context_t current_cell = sprite_selector_cells[current_sprite_index];
    Context_swap_pixels(current_cell, sprite_canvas_ctx);
    previous_rect_index = rect_index;
}

void tool_sprite_selection(const unsigned int rect_index)
{
    current_sprite_index = rect_index;
    Context_indicator_focus(sprite_selector_ctx, current_sprite_index);
    Context_swap_pixels(sprite_canvas_ctx, sprite_selector_cells[rect_index]);
}

static void tool_color_pick(const unsigned int rect_index)
{
    pen_color = rect_index;
}

void left_clicks()
{
    if (active_tool == FILL) {
        Context_handle_rect_click(sprite_canvas_ctx, tool_fill);
    } else if (active_tool == DRAG) {
        Context_handle_rect_click(sprite_canvas_ctx, tool_drag);
    } else {
        Context_handle_rect_click(sprite_canvas_ctx, tool_pen);
    }

    Context_handle_rect_click(color_picker_ctx, tool_color_pick);
    Context_handle_rect_click(sprite_selector_ctx, tool_sprite_selection);
    Context_handle_rect_click(toolbar_ctx, tool_toolbar_selection);
}

void right_clicks()
{
    Context_handle_rect_click(sprite_canvas_ctx, tool_alt_pen);
    Context_handle_rect_click(color_picker_ctx, tool_color_pick);
    Context_handle_rect_click(sprite_selector_ctx, tool_sprite_selection);
}

void copy_sprite()
{
    Message_Queue_enqueue(command_message_queue, "copied", 0);
    Context_to_pixel_buffer(sprite_canvas_ctx, clipboard_pixel_buffer);
    Context_swap_pixels(sprite_selector_cells[current_sprite_index], sprite_canvas_ctx);
}

void paste_sprite()
{
    Message_Queue_enqueue(command_message_queue, "paste", 0);
    Context_from_pixel_buffer(sprite_canvas_ctx, clipboard_pixel_buffer);
    Context_swap_pixels(sprite_selector_cells[current_sprite_index], sprite_canvas_ctx);
}

static void redo()
{
    Message_Queue_enqueue(command_message_queue, "redo", 0);
    Context_move_commits(sprite_canvas_ctx, 1);
    Context_indicator_focus(toolbar_ctx, PEN);
}

static void undo()
{
    Message_Queue_enqueue(command_message_queue, "undo", 0);
    Context_move_commits(sprite_canvas_ctx, -1);
}

void increment_sprite_selector()
{
    if (!(sprite_sheet_index_in_range(current_sprite_index + 1))) return;

    current_sprite_index++;
    tool_sprite_selection(current_sprite_index);
}

void decrement_sprite_selector()
{
    if (!(sprite_sheet_index_in_range(current_sprite_index - 1))) return;

    current_sprite_index--;
    tool_sprite_selection(current_sprite_index);
}

void increment_row_sprite_selector()
{
    if (!(sprite_sheet_index_in_range(current_sprite_index + SPRITESHEET_ROW_SIZE))) return;

    current_sprite_index += SPRITESHEET_ROW_SIZE;
    tool_sprite_selection(current_sprite_index);
}

void decrement_row_sprite_selector()
{
    if (!(sprite_sheet_index_in_range(current_sprite_index - SPRITESHEET_ROW_SIZE))) return;

    current_sprite_index -= SPRITESHEET_ROW_SIZE;
    tool_sprite_selection(current_sprite_index);
}

void draw_tool_activate_pen()
{
    active_tool = PEN;
    Message_Queue_enqueue(command_message_queue, "pen tool", 0);
    Context_indicator_focus(toolbar_ctx, PEN);
}

void draw_tool_activate_fill()
{
    active_tool = FILL;
    Message_Queue_enqueue(command_message_queue, "fill tool", 0);
    Context_indicator_focus(toolbar_ctx, FILL);
}

void draw_tool_activate_drag()
{
    active_tool = DRAG;
    Message_Queue_enqueue(command_message_queue, "drag tool", 0);
    Context_indicator_focus(toolbar_ctx, DRAG);
}

void draw_tool_handle_undo()
{
    undo();
    Context_indicator_focus(toolbar_ctx, PEN);
}

void draw_tool_handle_redo()
{
    redo();
    Context_indicator_focus(toolbar_ctx, PEN);
}

void draw_tool_handle_open_file()
{
    tool_sprite_selection(0);
    open_file();
}

static void tool_toolbar_selection(const unsigned int rect_index)
{
    switch(rect_index)
    {
        case PEN:
            draw_tool_activate_pen();
            break;
        case FILL:
            draw_tool_activate_fill();
            break;
        case DRAG:
            draw_tool_activate_drag();
            break;
        case UNDO:
            draw_tool_handle_undo();
            break;
        case REDO:
            draw_tool_handle_redo();
            break;
        case LOAD:
            draw_tool_handle_open_file();
            break;
        case SAVE:
            save_file(lshift);
            break;
        case INFO:
            show_help();
            break;
        default:
            break;
    }
}
