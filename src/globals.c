#include "defs.h"

SDL_Window *window;
SDL_Renderer *renderer;

void (*js_draw)(void);
void (*js_update)(void);
