#include "sprite_editor.h"
#include "canvas.h"
#include "color_picker.h"
#include "editor_tool.h"
#include "palette.h"
#include "pixel_buffer.h"
#include "sprite_picker.h"
#include <stdbool.h>
#include <stdlib.h>

#define SPRITE_PICKER_ENABLED 1

static sprite_editor current_editor_context = NULL;
void handle_mouse_click(sprite_editor e, int x, int y);
#if 0
static void init_toolbar(sprite_editor e, graphics g);
#endif

sprite_editor sprite_editor_create(graphics g) {
  sprite_editor e = malloc(sizeof(sprite_editor));
  if (!e)
    return NULL;

  int row_count = 8;
  float scale_factor = 42;

  e->graphics = g;
  e->canvas = canvas_create(g, row_count, row_count, scale_factor, 0, 0);

#if SPRITE_PICKER_ENABLED
  e->sprite_picker = sprite_picker_create(g, 8, 8, 3, 1, 420, 8, 8);
#endif

  e->color_picker = color_picker_create(g);
#if 0
  init_toolbar(e, g);
#endif

#if 1
  editor_tool_set_current_color(e->color_picker->pixel_buffer,
                                (coordinate){.x = 0, .y = 0},
                                &e->current_color);
#endif

  return e;
}

#if 0
static void init_toolbar(sprite_editor e, graphics g) {
  e->toolbar = grid_context_create(g, 1, 1, 2, 0, 420);
}
#endif

void sprite_editor_destroy(sprite_editor e) {
#if SPRITE_PICKER_ENABLED
  sprite_picker_destroy(e->sprite_picker);
#endif
  if (e->canvas != NULL) {
    canvas_destroy(e->canvas);
    e->canvas = NULL;
  }
  if (e->color_picker != NULL) {
    canvas_destroy(e->color_picker);
    e->color_picker = NULL;
  }
  // if (e != NULL) {
  //   free(e);
  //   e = NULL;
  // }
}

void sprite_editor_on_mouse_up(sprite_editor e, int x, int y) {}

void sprite_editor_update(void) {}

void sprite_editor_render(sprite_editor e) {
  graphics_set_draw_color(e->graphics, 74, 50, 110, 255);
  graphics_clear(e->graphics);

#if SPRITE_PICKER_ENABLED
  sprite_picker_render(e->sprite_picker);
#endif

  canvas_render(e->canvas);
  canvas_render(e->color_picker);

#if 0
  grid_context_render(e->toolbar);
#endif
  graphics_render_present(e->graphics);
}

void sprite_editor_on_mouse_down_left(sprite_editor e, int x, int y) {
  current_editor_context = e;
#if SPRITE_PICKER_ENABLED
  grid_context_on_mouse_down(e->sprite_picker->grid, NULL, x, y,
                             &e->current_color, editor_tool_select_sprite);
#endif
#if 0
  grid_context_on_mouse_down(e->canvas->grid, e->canvas->pixel_buffer, x, y,
                             &e->current_color, editor_tool_draw_to_canvas);
  grid_context_on_mouse_down(e->color_picker->grid,
                             e->color_picker->pixel_buffer, x, y,
                             &e->current_color, editor_tool_set_current_color);
#endif
  handle_mouse_click(e, x, y);
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
#if SPRITE_PICKER_ENABLED
  grid_context_on_mouse_down(e->sprite_picker->grid, NULL, x, y,
                             &e->current_color, editor_tool_select_sprite);
#endif
  grid_context_on_mouse_down(e->canvas->grid, e->canvas->pixel_buffer, x, y,
                             &e->current_color, editor_tool_draw_to_canvas);
  grid_context_on_mouse_down(e->color_picker->grid,
                             e->color_picker->pixel_buffer, x, y,
                             &e->current_color, editor_tool_set_current_color);
  handle_mouse_click(e, x, y);
}

void handle_mouse_click(sprite_editor e, int x, int y) {
  if (grid_context_is_within_grid_context(e->sprite_picker->grid, x, y)) {
    coordinate grid_coord =
        grid_context_screen_to_grid_coordinate(e->sprite_picker->grid, x, y);
    int index = grid_coord.y * e->sprite_picker->col_count + grid_coord.x;
    e->sprite_picker->selected_sprite = index;

    pixel_buffer_copy(
        e->sprite_picker->tiles[e->sprite_picker->selected_sprite],
        e->canvas->pixel_buffer);
  }

  if (grid_context_is_within_grid_context(e->canvas->grid, x, y)) {
    pixel_buffer_copy(
        e->canvas->pixel_buffer,
        e->sprite_picker->tiles[e->sprite_picker->selected_sprite]);
  }
}
