#ifndef _GLOBALS
#define _GLOBALS

#include "defs.h"

SDL_Window *window;
SDL_Renderer *renderer;

#include "context.h"
#include "util.h"

//TODO: move these into spritely
char main_color;
int current_sprite;
context sprite_canvas_ctx;
context color_picker_ctx;
context select_window_ctx;
context sprite_sheet_current_cell_ctx;
context sprite_sheet_cells[SPRITESHEET_SIZE];
uint sprite_sheet[SPRITESHEET_SIZE][SPRITE_CANVAS_SIZE];

#endif
