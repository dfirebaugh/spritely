#include "globals.h"

struct mouse mouse;

SDL_Window *window;
SDL_Renderer *renderer;

char pen_color;
int current_sprite_index;
int copy_index;
int lctrl;
int lshift;
color_t clipboard_pixel_buffer[SPRITE_CANVAS_SIZE];
Context_t sprite_canvas_ctx;
Context_t color_picker_ctx;
Context_t sprite_selector_ctx;
Context_t sprite_sheet_current_cell_ctx;
Context_t sprite_selector_cells[SPRITESHEET_SIZE];
Context_t color_selector_cells[COLORPICKER_CANVAS_SIZE];

SDL_Rect sprite_selection_indicator;
SDL_Rect color_picker_indicator;

uint sprite_sheet[SPRITESHEET_SIZE][SPRITE_CANVAS_SIZE];

Message_Queue_t command_message_queue;
Message_Queue_t help_message_queue;

unsigned int current_time;
unsigned int last_time;
