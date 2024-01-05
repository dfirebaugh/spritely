#ifndef PIXEL_BUFFER_H
#define PIXEL_BUFFER_H

#include <stdbool.h>

#include "color.h"
#include "graphics.h"

#define DEFAULT_COLOR BLACK

// pixel_buffer encapsulates an in-memory image framebuffer.
typedef struct {
  RGBA **img;
  int width, height;
} *pixel_buffer;

extern pixel_buffer pixel_buffer_create(int width, int height);
extern pixel_buffer pixel_buffer_from_hex(const char *hex, RGBA *palette);
extern void pixel_buffer_destroy(pixel_buffer pb);
extern void pixel_buffer_set_pixel(pixel_buffer pb, int x, int y, RGBA c);
extern RGBA pixel_buffer_get_rgba(pixel_buffer pb, int x, int y);
extern void pixel_buffer_get_size(pixel_buffer pb, int *width, int *height);
extern bool is_row_of_color(RGBA **img, int y, int width, RGBA color);
extern Rect *pixel_buffer_to_rects(pixel_buffer pb, int *num_rects,
                                   int scale_factor, int offset_x,
                                   int offset_y);
extern void pixel_buffer_fill(pixel_buffer pb, RGBA c);
extern void pixel_buffer_copy(pixel_buffer src, pixel_buffer dest);
extern int pixel_buffer_get_height(pixel_buffer pb);
extern int pixel_buffer_get_width(pixel_buffer pb);
extern void pixel_buffer_render(pixel_buffer pb, graphics gfx, int scale_factor,
                                int offset_x, int offset_y);

#endif // PIXEL_BUFFER_H
