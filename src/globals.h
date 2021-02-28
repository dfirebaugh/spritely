#ifndef _GLOBALS
#define _GLOBALS

#include "defs.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;

/**
 * js_draw is a Function pointer used to register a javascript function
 *  to call during the render loop
 */
extern void (*js_draw)(void);
/**
 * js_update is a Function pointer used to register a javascript function
 *  to call during the update loop
 */
extern void (*js_update)(void);


#endif
