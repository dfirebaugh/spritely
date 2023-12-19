#ifndef CANVAS_H
#define CANVAS_H

#include "graphics.h"
#include "grid_context.h"
#include "pixel_buffer.h"

typedef struct {
  graphics graphics;
  pixel_buffer pixel_buffer;
  grid_context grid;
  bool has_indicator;
} *canvas;

extern canvas canvas_create(const graphics gfx, int col_count, int row_count,
                            int scale_factor, int offset_x, int offset_y);
extern void canvas_destroy(canvas c);
extern void canvas_render(canvas c);

#endif // CANVAS_H
