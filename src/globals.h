#include "defs.h"

#ifndef _GLOBALS
#define _GLOBALS

SDL_Window *window;
SDL_Renderer *renderer;

struct mouse {
    int x;
    int y;
} mouse;

typedef struct pixel {
    unsigned int color;
    SDL_Rect rect;
} pixel;


char main_color;

#endif
