#ifndef sprite_sheet_H
#define sprite_sheet_H

#include "canvas.h"
#include "grid_context.h"
#include "pixel_buffer.h"

typedef struct {
  graphics graphics;
  grid_context grid;
  offset offset;
  int col_count;
  int row_count;
  int scale_factor;
  int selected_sprite;
  pixel_buffer copy_buffer;
  pixel_buffer *tiles;
  int tile_count;
} *sprite_sheet;

extern sprite_sheet sprite_sheet_create(graphics gfx, int col_count,
                                        int row_count, int scale_factor,
                                        int offset_x, int offset_y,
                                        int sprite_col_count,
                                        int sprite_row_count);
extern void sprite_sheet_destroy(sprite_sheet sp);
extern void sprite_sheet_render(sprite_sheet sp);

#endif // sprite_sheet_H
