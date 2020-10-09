#ifndef _GLOBALS
#define _GLOBALS

struct mouse
{
    int x;
    int y;
};

extern struct mouse mouse;

#include "defs.h"
#include "colors.h"
#include "message_queue.h"
#include "context.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;

//TODO: move these into spritely
extern char pen_color;
extern int current_sprite_index;
extern int copy_index;
extern int lctrl;
extern int lshift;
extern color_t clipboard_pixel_buffer[SPRITE_CANVAS_SIZE];
extern Context_t sprite_canvas_ctx;
extern Context_t color_picker_ctx;
extern Context_t sprite_selector_ctx;
extern Context_t sprite_sheet_current_cell_ctx;
extern Context_t toolbar_ctx;
extern Context_t sprite_selector_cells[SPRITESHEET_SIZE];
extern Context_t color_selector_cells[COLORPICKER_CANVAS_SIZE];

extern uint sprite_sheet[SPRITESHEET_SIZE][SPRITE_CANVAS_SIZE];

extern Message_Queue_t command_message_queue;
extern Message_Queue_t help_message_queue;

extern unsigned int current_time;
extern unsigned int last_time;

typedef enum tool_types {
  PEN = 0,
  FILL
} tool_t;

extern tool_t active_tool;

#endif
