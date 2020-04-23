#ifndef _GLOBALS
#define _GLOBALS

struct mouse
{
    int x;
    int y;
} mouse;

#include "defs.h"
#include "colors.h"
#include "context.h"

SDL_Window *window;
SDL_Renderer *renderer;

//TODO: move these into spritely
char pen_color;
int current_sprite_index;
int copy_index;
int lctrl;
color_t clipboard_pixel_buffer[SPRITE_CANVAS_SIZE];
Context_T sprite_canvas_ctx;
Context_T color_picker_ctx;
Context_T sprite_selector_ctx;
Context_T sprite_sheet_current_cell_ctx;
Context_T sprite_selector_cells[SPRITESHEET_SIZE];
Context_T sprite_selection_indicator;
// Message_box_T *current_mbox;

uint sprite_sheet[SPRITESHEET_SIZE][SPRITE_CANVAS_SIZE];
unsigned int current_time;
unsigned int last_time;

#endif
