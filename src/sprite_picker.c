
#include <stdlib.h>
#include <string.h>

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

  // Initialization
  sp->graphics = gfx;
  sp->selected_sprite = 0;
  sp->grid = grid_context_create(gfx, col_count, row_count, scale_factor,
                                 offset_x, offset_y);
  grid_enable_indicator(sp->grid);

  int nelements = col_count * row_count;

  sp->pb = (pixel_buffer *)malloc(nelements * sizeof(pixel_buffer));
  for (int i = 0; i < nelements; i++) {
    pixel_buffer pb = pixel_buffer_create(sprite_col_count, sprite_row_count);
    RGBA rgba = {0, 0, 0, 255};
    pixel_buffer_fill(pb, rgba);
    sp->pb[i] = pb;
  }

  return sp;
}

void sprite_picker_destroy(sprite_picker sp) {
  int nelements = sp->grid->col_count * sp->grid->row_count;
  for (int i = 0; i < nelements; i++) {
    pixel_buffer_destroy(sp->pb[i]);
  }
  grid_context_destroy(sp->grid);
  free(sp->pb);
  free(sp);
}

void sprite_picker_render(sprite_picker sp) {
  if (!sp)
    return;

  if (!sp->pb)
    return;

  int nelements = sp->grid->col_count * sp->grid->row_count;
  for (int i = 0; i < nelements; i++) {
    if (!sp->pb[i]) {
      continue;
    }

    pixel_buffer pb = sp->pb[i];

    int row = i / sp->grid->col_count;
    int col = i % sp->grid->col_count;
    int render_x = col * sp->grid->scale_factor + sp->grid->offset_x;
    int render_y = row * sp->grid->scale_factor + sp->grid->offset_y;

    pixel_buffer_render(pb, sp->graphics, sp->grid->scale_factor, render_x,
                        render_y);
  }
  grid_context_render(sp->grid);
}
