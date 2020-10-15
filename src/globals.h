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
#include "util.h"
#include "file.h"
#include "draw_tools.h"
#include "sprite_sheet.h"
#include "app_state.h"
#include "sprite_editor.h"
#include "shell.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern unsigned int spritely_editor_initialized;

extern Shell_t spritely_shell;

//TODO: move these into spritely
extern unsigned int pen_color;
extern unsigned int current_sprite_index;
extern unsigned int copy_index;
extern unsigned int lctrl;
extern unsigned int lshift;
extern App_State_t spritely_state;
extern color_t clipboard_pixel_buffer[SPRITE_CANVAS_SIZE];
extern Context_t sprite_canvas_ctx;
extern Context_t color_picker_ctx;
extern Context_t sprite_selector_ctx;
extern Context_t sprite_sheet_current_cell_ctx;
extern Context_t toolbar_ctx;
extern Context_t sprite_selector_cells[SPRITESHEET_SIZE];
extern Context_t color_selector_cells[COLORPICKER_CANVAS_SIZE];

extern Sprite_sheet_t icon_sprite_sheet;
extern Sprite_sheet_t main_font_sprite_sheet;

extern uint sprite_sheet[SPRITESHEET_SIZE][SPRITE_CANVAS_SIZE];

extern Message_Queue_t command_message_queue;
extern Message_Queue_t help_message_queue;

extern unsigned int current_time;
extern unsigned int last_time;

extern unsigned int batch_operation_counter;

typedef enum tool_types {
  PEN = 0,
  FILL,
  DRAG,
  UNDO,
  REDO,
  LOAD,
  SAVE,
  INFO
} tool_t;


extern char icon_files[TOOLBAR_ROW_SIZE][50];

extern tool_t active_tool;

extern void increment_batch_operation_count(void);

extern void reset_batch_operation_counter(void);

extern void free_globals(void);

#endif
