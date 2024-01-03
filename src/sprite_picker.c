
#include <stdlib.h>
#include <string.h>

#include "apprt.h"
#include "graphics.h"
#include "grid_context.h"
#include "pixel_buffer.h"
#include "sprite_picker.h"

sprite_picker sprite_picker_create(graphics gfx, int col_count, int row_count,
                                   int scale_factor, int offset_x, int offset_y,
                                   int sprite_col_count, int sprite_row_count) {
  sprite_picker sp = malloc(sizeof(sprite_picker));
  if (!sp)
    return NULL;

  sp->graphics = gfx;
  sp->selected_sprite = 0;
  sp->offset.y = offset_y;
  sp->offset.x = offset_x;
  sp->scale_factor = scale_factor;
  sp->grid = grid_context_create(gfx, col_count, row_count, scale_factor,
                                 offset_x, offset_y);
  grid_enable_indicator(sp->grid);

  sp->col_count = col_count;
  sp->row_count = row_count;

#if 1
  sp->tile_count = col_count * row_count;
  sp->tiles = calloc(sp->tile_count, sp->tile_count * sizeof(canvas));
  for (int i = 0; i < sp->tile_count; i++) {
    sp->tiles[i] = pixel_buffer_create(sprite_row_count, sprite_col_count);
    RGBA rgba = {0, 0, 0, 255};
    pixel_buffer_fill(sp->tiles[i], rgba);
  }
#endif

  return sp;
}

void sprite_picker_destroy(sprite_picker sp) {
  if (sp->grid != NULL) {
    grid_context_destroy(sp->grid);
    sp->grid = NULL;
  }

#if 1
  if (sp->tiles != NULL) {
    for (int i = 0; i < sp->tile_count; i++) {
      pixel_buffer_destroy(sp->tiles[i]);
    }
    free(sp->tiles);
    sp->tiles = NULL;
  }
#endif

  // if (sp != NULL) {
  //   free(sp);
  //   sp = NULL;
  // }
}

void sprite_picker_render(sprite_picker sp) {
  if (!sp || !sp->tiles)
    return;
#if 1

  int scale_factor = sp->scale_factor / sp->col_count;
  int sprite_col_count = 8;
  int sprite_row_count = 8;
  int sprite_width = sprite_col_count;
  int sprite_height = sprite_row_count;

  for (int i = 0; i < sp->tile_count; i++) {
    if (!sp->tiles[i]) {
      continue;
    }

    int col = i % sp->col_count * (scale_factor * sprite_width);
    int row = i / sp->col_count * (scale_factor * sprite_height);
    int x = col + sp->offset.x;
    int y = row + sp->offset.y;

    pixel_buffer_render(sp->tiles[i], sp->graphics, scale_factor, x, y);
  }
#endif

  if (!sp->grid)
    return;

  grid_context_render(sp->grid);
}
