#ifndef _GLOBALS
#define _GLOBALS

#include "defs.h"
#include "colors.h"
#include "context.h"

SDL_Window *window;
SDL_Renderer *renderer;

//TODO: move these into spritely
char main_color;
int current_sprite;
int copy_index;
int lctrl;
color_t pixel_buffer[SPRITE_CANVAS_SIZE];
context sprite_canvas_ctx;
context color_picker_ctx;
context select_window_ctx;
context sprite_sheet_current_cell_ctx;
context sprite_sheet_cells[SPRITESHEET_SIZE];
context sprite_selection_cursor;
uint sprite_sheet[SPRITESHEET_SIZE][SPRITE_CANVAS_SIZE];

#endif
