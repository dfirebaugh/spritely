#include "globals.h"
#include "util.h"

#ifndef _CONTEXT
#define _CONTEXT

typedef struct context{
    uint pixel_size;
    uint canvas_size;
    uint row_size;
    uint col_size;
    uint x_offset;
    uint y_offset;
    color_t pixels[SPRITE_CANVAS_SIZE];
    SDL_Rect rects[SPRITE_CANVAS_SIZE];
} context;

/* context_make -- construct a context of pixles scaled based on size passed in */
extern context context_make(uint pixel_size, uint row_size, uint col_size, uint x_offset, uint y_offset);

/* context_render renders the pixels based on their size */
extern void context_render(context *ctx);

/* context_handle_rect_click - loops through the rects in the context and determines 
*   if the current mouse coordinates are on top of a specific rect 
*   if it is within the rect, the function will execute a function pointer and pass through the 
*   index of that rect.
*/
extern void context_handle_rect_click(const context ctx, void  (*fn)(const unsigned char));

/* swap the pixels from one context to another */
extern void context_swap_pixels(context *dest, context *source);

/* moves a context to the position of another context's first pixel.  This is good for indicators 
*   (i.e. move one pixel on tope of another)
*   (e.g. the sprite selector has an indicator that follows where we click the mouse to show which sprite we are currently editing)
*/
extern void context_focus(context *dest, context *source);

#endif