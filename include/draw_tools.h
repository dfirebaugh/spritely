#ifndef _DRAWTOOLS
#define _DRAWTOOLS
#include "context.h"
#include "message_queue.h"
typedef struct mouse {
  int x;
  int y;
  int canvas_x;
  int canvas_y;
} mouse_t;

extern mouse_t mouse;

extern Context_t sprite_canvas_ctx;
extern Context_t color_picker_ctx;
extern Context_t sprite_selector_ctx;
extern Context_t sprite_sheet_current_cell_ctx;
extern Context_t toolbar_ctx;
extern Context_t sprite_selector_cells[SPRITESHEET_SIZE];
extern Context_t color_selector_cells[COLORPICKER_CANVAS_SIZE];

extern Message_Queue_t command_message_queue;
extern Message_Queue_t help_message_queue;

extern uint sprite_sheet[SPRITESHEET_SIZE][SPRITE_CANVAS_SIZE];

extern const char *palette_file;

extern unsigned int pen_color;
extern unsigned int lctrl;
extern unsigned int lshift;

extern void draw_tool_handle_open_file(void);
extern void draw_tool_handle_undo(void);
extern void draw_tool_handle_redo(void);
extern void draw_tool_activate_fill(void);
extern void draw_tool_activate_pen(void);
extern void decrement_row_sprite_selector(void);
extern void increment_row_sprite_selector(void);
extern void decrement_sprite_selector(void);
extern void increment_sprite_selector(void);
extern void copy_sprite(void);
extern void paste_sprite(void);
extern void right_clicks(void);
extern void left_clicks(void);
extern void left_drags(void);
extern void show_help(void);
extern void tool_sprite_selection(const unsigned int rect_index);

#endif
