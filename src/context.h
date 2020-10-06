#ifndef _CONTEXT
#define _CONTEXT

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
extern void Context_handle_rect_click(Context_t ctx, void  (*fn)(const unsigned char));

/* swap the pixels from one Context to another */
extern void Context_swap_pixels(Context_t dest, Context_t source);

/* moves a Context to the position of another Context's first pixel.  This is good for indicators
*   (i.e. move one pixel on tope of another)
*   (e.g. the sprite selector has an indicator that follows where we click the mouse to show which sprite we are currently editing)
*/
extern void Context_focus(Context_t dest, Context_t source);
extern void Context_from_pixel_buffer(Context_t ctx, color_t *pixel_buffer);
extern void Context_to_pixel_buffer(Context_t ctx, color_t *pixel_buffer);
extern color_t Context_get_pixel (Context_t ctx, const unsigned char pixel_index);
extern void Context_set_pixel(Context_t ctx, const unsigned char pixel_index, color_t color);
extern void Context_indicator_focus(SDL_Rect *indicator, Context_t ctx, const unsigned char rect_index);

extern void Context_free_future_commits(Context_t ctx);
extern void Context_new_commit(Context_t ctx, color_t pre_color, color_t post_color, uint position);
extern void Context_move_commits(Context_t ctx, int offset);

/* Return 1 if the ctx is entirely the given color else return 0 */
extern int Context_is_solid_color(Context_t ctx, color_t color);

#endif
