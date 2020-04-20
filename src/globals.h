#ifndef _GLOBALS
#define _GLOBALS

#include "defs.h"
#include "pixel.h"

SDL_Window *window;
SDL_Renderer *renderer;

struct mouse {
    int x;
    int y;
} mouse;

char main_color;

#include "context.h"
#include "util.h"
#endif
