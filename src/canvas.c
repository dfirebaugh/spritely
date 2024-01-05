
#include <stdlib.h>

#include "color.h"
#include "graphics.h"
#include "grid_context.h"
#include "pixel_buffer.h"

#include "canvas.h"

canvas canvas_create(const graphics gfx, int col_count, int row_count,
                     int scale_factor, int offset_x, int offset_y) {
  canvas c = malloc(sizeof(canvas));
  if (!c)
    return NULL;

  c->graphics = gfx;
  c->grid = grid_context_create(gfx, row_count, row_count, scale_factor,
                                offset_x, offset_y);
  if (!c->grid) {
    free(c);
    return NULL;
  }
  c->grid->has_indicator = false;

  c->pixel_buffer = pixel_buffer_create(c->grid->col_count, c->grid->row_count);
  RGBA rgba = {0, 0, 0, 255};
  pixel_buffer_fill(c->pixel_buffer, rgba);

  return c;
}

void canvas_destroy(canvas c) {
  if (!c)
    return;
  pixel_buffer_destroy(c->pixel_buffer);
  grid_context_destroy(c->grid);
  free(c);
}

void canvas_render(canvas c, graphics g) {
  if (!c)
    return;

  pixel_buffer_render(c->pixel_buffer, c->graphics, c->grid->scale_factor,
                      c->grid->offset_x, c->grid->offset_y);
  grid_context_render(c->grid, g);
}
