#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "grid_context.h"
#include "pixel_buffer.h"
#include "sprite_editor.h"

coordinate grid_to_screen_coordinate(grid_context w, coordinate local_coord);

grid_context grid_context_create(graphics gfx, int col_count, int row_count,
                                 int scale_factor, int offset_x, int offset_y) {
  grid_context w = malloc(sizeof(grid_context));
  if (!w)
    return NULL;

  w->graphics = gfx;
  w->total_elements = col_count * row_count;
  w->col_count = col_count;
  w->row_count = row_count;
  w->scale_factor = scale_factor;
  w->offset_x = offset_x;
  w->offset_y = offset_y;
  w->has_indicator = false;
  w->selected = (coordinate){.x = 0, .y = 0};

  w->pixel_buffer = pixel_buffer_create(w->col_count, w->row_count);
  RGBA c = {0, 0, 0, 255};
  pixel_buffer_fill(w->pixel_buffer, c);

  return w;
}

void grid_context_destroy(grid_context w) {
  if (!w)
    return;
  pixel_buffer_destroy(w->pixel_buffer);
  free(w);
}

void draw_pixel(graphics g, int x, int y, RGBA c) {}

void draw_indicator(grid_context w) {
  if (!w->has_indicator)
    return;

  coordinate screen_coord = grid_to_screen_coordinate(w, w->selected);
  Rect r;
  r.x = screen_coord.x;
  r.y = screen_coord.y;
  r.w = w->scale_factor;
  r.h = w->scale_factor;
  graphics_set_draw_color(w->graphics, 255, 255, 255, 255);
  graphics_draw_rect(w->graphics, &r);
}

void grid_context_render(grid_context w) {
  if (!w)
    return;

  draw_indicator(w);
}

void grid_enable_indicator(grid_context w) { w->has_indicator = true; }

void grid_disable_indicator(grid_context w) { w->has_indicator = false; }

void grid_set_offset(grid_context w, offset o) { w->offset = o; }

coordinate grid_to_screen_coordinate(grid_context w, coordinate local_coord) {
  return (coordinate){.x = (local_coord.x * w->scale_factor) + w->offset_x,
                      .y = (local_coord.y * w->scale_factor) + w->offset_y};
}

coordinate grid_to_local_coordinate(grid_context w, coordinate c) {
  return (coordinate){.x = (c.x - w->offset_x) / w->scale_factor,
                      .y = (c.y - w->offset_y) / w->scale_factor};
}

static bool grid_context_is_within_bounds(grid_context w, coordinate c) {
  if (!w)
    return false;
  coordinate local_coord = grid_to_local_coordinate(w, c);

  return local_coord.x >= 0 && local_coord.x < w->col_count &&
         local_coord.y >= 0 && local_coord.y < w->row_count;
}

void grid_context_on_mouse_down(grid_context w, pixel_buffer pb, float click_x,
                                float click_y, color *current_color,
                                mouse_down_callback callback) {
  coordinate coord = {click_x, click_y};
  if (!grid_context_is_within_bounds(w, coord))
    return;

  coordinate local_coord = grid_to_local_coordinate(w, coord);
  w->selected = local_coord;
  callback(pb, local_coord, current_color);
}

coordinate grid_context_screen_to_grid_coordinate(grid_context w, int screen_x,
                                                  int screen_y) {
  coordinate grid_coord;
  int adjusted_x = screen_x - w->offset_x;
  int adjusted_y = screen_y - w->offset_y;

  grid_coord.x = adjusted_x / w->scale_factor;
  grid_coord.y = adjusted_y / w->scale_factor;

  return grid_coord;
}

bool grid_context_is_within_grid_context(grid_context w, int screen_x,
                                         int screen_y) {
  coordinate local_coord =
      grid_context_screen_to_grid_coordinate(w, screen_x, screen_y);

  return local_coord.x >= 0 && local_coord.x < w->col_count &&
         local_coord.y >= 0 && local_coord.y < w->row_count;
}
