#ifndef _GLOBALS
#define _GLOBALS

#include "defs.h"

SDL_Window *window;
SDL_Renderer *renderer;

typedef struct pixel {
    int color;
    SDL_Rect rect;
} pixel;

struct mouse {
    int x;
    int y;
} mouse;


char main_color;

#include "context.h"
#include "util.h"

context sprite_canvas_ctx;
context color_picker_ctx;
int current_sprite;
context select_window_ctx;

context sprite_sheet_current_cell_ctx;
context sprite_sheet_cells[SPRITESHEET_SIZE];
uint sprite_sheet[SPRITESHEET_SIZE][SPRITE_CANVAS_SIZE];

#endif
