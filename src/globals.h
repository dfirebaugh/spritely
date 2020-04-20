#include "defs.h"

#ifndef _GLOBALS
#define _GLOBALS

typedef struct pixel {
    unsigned int color;
    SDL_Rect rect;
} pixel;

SDL_Window *window;
SDL_Renderer *renderer;

struct mouse {
    int x;
    int y;
} mouse;


char main_color;

#endif
