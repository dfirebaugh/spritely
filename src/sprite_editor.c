#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "editor_tool.h"
#include "event.h"
#include "graphics.h"
#include "input.h"
#include "palette.h"
#include "pixel_buffer.h"
#include "sprite_editor.h"
#if 1
#include "sprite_picker.h"
#endif
#include "state.h"

static sprite_editor current_editor_context = NULL;

static void init_color_picker(sprite_editor e, graphics g);
static void init_toolbar(sprite_editor e, graphics g);

sprite_editor sprite_editor_create(graphics g) {
  sprite_editor e = malloc(sizeof(struct editor));
  if (!e)
    return NULL;

  int row_count = 8;
  float scale_factor = 42;

  e->graphics = g;
  e->canvas = canvas_create(g, row_count, row_count, scale_factor, 0, 0);

#if 1
  e->sprite_picker = sprite_picker_create(g, 16, 2, 16, 0, 420, 8, 8);
#endif

  init_color_picker(e, g);
  init_toolbar(e, g);

  return e;
}

static void init_toolbar(sprite_editor e, graphics g) {
  e->toolbar = grid_context_create(g, 1, 1, 2, 0, 420);
}

static void init_color_picker(sprite_editor e, graphics g) {
  if (!load_palette("./assets/palette/palette.cfg")) {
    printf("Failed to load palette!\n");
  }
  e->color_picker = canvas_create(g, 4, 4, 44, 600, 64);
  grid_enable_indicator(e->color_picker->grid);

  for (int index = 0; index < COLOR_PICKER_NUM_COLORS; index++) {
    int x = index % 4;
    int y = index / 4;

    color col_enum_value = (x + y * 4) % COLOR_PICKER_NUM_COLORS;
    pixel_buffer_set_pixel(e->color_picker->pixel_buffer, x, y,
                           color_to_rgba(col_enum_value));
  }

  coordinate first_color;
  first_color.x = 0;
  first_color.y = 0;
  editor_tool_set_current_color(e->color_picker->pixel_buffer, first_color,
                                &e->current_color);
}

void sprite_editor_destroy(sprite_editor e) {
  canvas_destroy(e->canvas);
#if 1
  sprite_picker_destroy(e->sprite_picker);
#endif
  free(e);
}

void sprite_editor_on_mouse_up(sprite_editor e, int x, int y) {}

void sprite_editor_update(void) {}

void sprite_editor_render(sprite_editor e) {
  graphics_set_draw_color(e->graphics, 74, 50, 110, 255);
  graphics_clear(e->graphics);

#if 1
  sprite_picker_render(e->sprite_picker);
#endif

  canvas_render(e->canvas);
  canvas_render(e->color_picker);

  grid_context_render(e->toolbar);
  graphics_render_present(e->graphics);
}

void sprite_editor_on_mouse_down_left(sprite_editor e, int x, int y) {
  current_editor_context = e;
#if 1
  grid_context_on_mouse_down(e->sprite_picker->grid, NULL, x, y,
                             &e->current_color, editor_tool_select_sprite);
#endif
  grid_context_on_mouse_down(e->canvas->grid, e->canvas->pixel_buffer, x, y,
                             &e->current_color, editor_tool_draw_to_canvas);
  grid_context_on_mouse_down(e->color_picker->grid,
                             e->color_picker->pixel_buffer, x, y,
                             &e->current_color, editor_tool_set_current_color);
}
void sprite_editor_on_mouse_down_right(sprite_editor e, int x, int y) {
  grid_context_on_mouse_down(e->canvas->grid, e->canvas->pixel_buffer, x, y,
                             &e->current_color, editor_tool_set_current_color);
  grid_context_on_mouse_down(e->color_picker->grid,
                             e->color_picker->pixel_buffer, x, y,
                             &e->current_color, editor_tool_set_current_color);
}

void sprite_editor_on_mouse_up_left(sprite_editor e, int x, int y) {}
void sprite_editor_on_mouse_up_right(sprite_editor e, int x, int y) {}

void sprite_editor_on_mouse_move(sprite_editor e, int x, int y) {
  if (!input_is_mouse_down(e->input))
    return;
#if 1
  grid_context_on_mouse_down(e->sprite_picker->grid, NULL, x, y,
                             &e->current_color, editor_tool_select_sprite);
#endif
  grid_context_on_mouse_down(e->canvas->grid, e->canvas->pixel_buffer, x, y,
                             &e->current_color, editor_tool_draw_to_canvas);
  grid_context_on_mouse_down(e->color_picker->grid,
                             e->color_picker->pixel_buffer, x, y,
                             &e->current_color, editor_tool_set_current_color);
}
