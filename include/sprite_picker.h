#ifndef SPRITE_PICKER_H
#define SPRITE_PICKER_H

#include "canvas.h"
#include "grid_context.h"
#include "pixel_buffer.h"

typedef struct {
  graphics graphics;
  grid_context grid;
  // coordinate selected;
  offset offset;
  int col_count;
  int row_count;
  int scale_factor;
  int selected_sprite;
  pixel_buffer *tiles;
  int tile_count;
} *sprite_picker;

extern sprite_picker sprite_picker_create(graphics gfx, int col_count,
                                          int row_count, int scale_factor,
                                          int offset_x, int offset_y,
                                          int sprite_col_count,
                                          int sprite_row_count);
extern void sprite_picker_destroy(sprite_picker sp);
extern void sprite_picker_render(sprite_picker sp);

#endif // SPRITE_PICKER_H
