
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "grid_context.h"
#include "pixel_buffer.h"
#include "sprite_sheet.h"

sprite_sheet sprite_sheet_create(graphics gfx, int col_count, int row_count,
                                 int scale_factor, int offset_x, int offset_y,
                                 int sprite_col_count, int sprite_row_count) {
  sprite_sheet s = malloc(sizeof(sprite_sheet));
  if (!s)
    return NULL;

  s->graphics = gfx;
  s->selected_sprite = 0;
  s->offset.y = offset_y;
  s->offset.x = offset_x;
  s->scale_factor = scale_factor;
  s->grid = grid_context_create(gfx, col_count, row_count,
                                scale_factor * col_count, offset_x, offset_y);
  grid_enable_indicator(s->grid);

  s->col_count = col_count;
  s->row_count = row_count;

  s->copy_buffer = pixel_buffer_create(sprite_row_count, sprite_col_count);
  RGBA rgba = {0, 0, 0, 255};
  pixel_buffer_fill(s->copy_buffer, rgba);

#if 1
  s->tile_count = col_count * row_count;
  s->tiles = calloc(s->tile_count, s->tile_count * sizeof(canvas));
  for (int i = 0; i < s->tile_count; i++) {
    s->tiles[i] = pixel_buffer_create(sprite_row_count, sprite_col_count);
    RGBA rgba = {0, 0, 0, 255};
    pixel_buffer_fill(s->tiles[i], rgba);
  }
#endif

  return s;
}

void sprite_sheet_destroy(sprite_sheet s) {
  if (s->grid != NULL) {
    grid_context_destroy(s->grid);
    s->grid = NULL;
  }

#if 1
  if (s->tiles != NULL) {
    for (int i = 0; i < s->tile_count; i++) {
      pixel_buffer_destroy(s->tiles[i]);
    }
    free(s->tiles);
    s->tiles = NULL;
  }
#endif

  // if (sp != NULL) {
  //   free(sp);
  //   sp = NULL;
  // }
}

void sprite_sheet_render(sprite_sheet s) {
  if (!s || !s->tiles)
    return;
#if 1

  int scale_factor = s->scale_factor;
  int sprite_col_count = s->col_count;
  int sprite_row_count = s->row_count;
  int sprite_width = sprite_col_count;
  int sprite_height = sprite_row_count;

  for (int i = 0; i < s->tile_count; i++) {
    if (!s->tiles[i]) {
      continue;
    }

    int col = i % s->col_count * (scale_factor * sprite_width);
    int row = i / s->col_count * (scale_factor * sprite_height);
    int x = col + s->offset.x;
    int y = row + s->offset.y;

    pixel_buffer_render(s->tiles[i], s->graphics, scale_factor, x, y);
  }
#endif

  if (!s->grid)
    return;

  grid_context_render(s->grid);
}
