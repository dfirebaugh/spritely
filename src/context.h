#ifndef _CONTEXT
#define _CONTEXT
#include "sprite_sheet.h"

typedef struct Context *Context_t;

/* Context_make -- construct a Context of pixles scaled based on size passed in */
extern Context_t Context_make(uint pixel_size, uint row_size, uint col_size, uint x_offset, uint y_offset);

/* free up the memory that the Context is taking up */
extern void Context_free(Context_t ctx);


/* Context_render renders the pixels based on their size */
extern void Context_render(Context_t ctx);

/* Context_handle_rect_click - loops through the rects in the Context and determines
*   if the current mouse coordinates are on top of a specific rect
*   if it is within the rect, the function will execute a function pointer and pass through the
*   index of that rect.
*/
extern void Context_handle_rect_click(Context_t ctx, void  (*fn)(const unsigned int));

/* swap the pixels from one Context to another */
extern void Context_swap_pixels(Context_t dest, Context_t source);

extern void Context_from_pixel_buffer(Context_t ctx, color_t *pixel_buffer);
extern void Context_to_pixel_buffer(Context_t ctx, color_t *pixel_buffer);
extern color_t Context_get_pixel (Context_t ctx, const unsigned int pixel_index);
extern void Context_set_pixel(Context_t ctx, const unsigned int pixel_index, color_t color);
extern void Context_indicator_focus(Context_t ctx, const unsigned int rect_index);

/**
 * Context_make_indicator
 * make an indicator for the context.  When a cell in the context is clicked,
 * it will be highlighted.
 */
extern void Context_make_indicator(Context_t ctx);

extern void Context_free_future_commits(Context_t ctx);
extern void Context_new_commit(Context_t ctx, color_t pre_color, color_t post_color, uint position);
extern void Context_move_commits(Context_t ctx, int offset);

/**
 * Context_make_transparent - the context will not render if this is executed, 
 * but the indicator will render
 */
extern void Context_make_transparent(Context_t ctx);

/* Return 1 if the ctx is entirely the given color else return 0 */
extern int Context_is_solid_color(Context_t ctx, color_t color);


/**
 * Context_render_sprite_in_context
 * renders a specific sprite from a sprite sheet in a context cell
 * this is useful if you need to handle click events (i.e. icons that trigger something)
*/
extern void Context_render_sprite_in_context(Context_t ctx, Sprite_sheet_t sprite_sheet, uint index, uint context_index);

#endif
