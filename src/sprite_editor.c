#include "sprite_editor.h"
#include "canvas.h"
#include "color_picker.h"
#include "editor_tool.h"
#include "input.h"
#include "palette.h"
#include "pixel_buffer.h"
#include "sprite_sheet.h"
#include <stdbool.h>
#include <stdlib.h>

#define sprite_sheet_ENABLED 1

#if 0
static void init_toolbar(sprite_editor e, graphics g);
#endif

sprite_editor sprite_editor_create(graphics g) {
  sprite_editor e = malloc(sizeof(*e));
  if (!e)
    return NULL;

  int row_count = 8;
  float scale_factor = 42;

  // e->graphics = g;
  e->canvas = canvas_create(g, row_count, row_count, scale_factor, 0, 0);

#if sprite_sheet_ENABLED
  e->sprite_sheet = sprite_sheet_create(g, 8, 8, 3, 1, 420, 8, 8);
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

  if (!e->canvas || !e->sprite_sheet || !e->color_picker) {
    if (e->canvas)
      canvas_destroy(e->canvas);
    if (e->sprite_sheet)
      sprite_sheet_destroy(e->sprite_sheet);
    if (e->color_picker)
      canvas_destroy(e->color_picker);
    free(e);
    return NULL;
  }

  return e;
}

#if 0
static void init_toolbar(sprite_editor e, graphics g) {
  e->toolbar = grid_context_create(g, 1, 1, 2, 0, 420);
}
#endif

void sprite_editor_destroy(sprite_editor e) {
#if sprite_sheet_ENABLED
  sprite_sheet_destroy(e->sprite_sheet);
#endif
  if (e->canvas != NULL) {
    canvas_destroy(e->canvas);
    e->canvas = NULL;
  }
  if (e->color_picker != NULL) {
    canvas_destroy(e->color_picker);
    e->color_picker = NULL;
  }
  if (e != NULL) {
    free(e);
    e = NULL;
  }
}

void handle_mouse_click(sprite_editor e, int x, int y) {
#if sprite_sheet_ENABLED
  grid_context_on_mouse_down(e->sprite_sheet->grid, NULL, x, y,
                             &e->current_color, editor_tool_select_sprite);
#endif
  grid_context_on_mouse_down(e->canvas->grid, e->canvas->pixel_buffer, x, y,
                             &e->current_color, editor_tool_draw_to_canvas);
  grid_context_on_mouse_down(e->color_picker->grid,
                             e->color_picker->pixel_buffer, x, y,
                             &e->current_color, editor_tool_set_current_color);

  if (grid_context_is_within_grid_context(e->sprite_sheet->grid, x, y)) {
    coordinate grid_coord =
        grid_context_screen_to_grid_coordinate(e->sprite_sheet->grid, x, y);
    int index = grid_coord.y * e->sprite_sheet->col_count + grid_coord.x;
    e->sprite_sheet->selected_sprite = index;

    pixel_buffer_copy(e->sprite_sheet->tiles[e->sprite_sheet->selected_sprite],
                      e->canvas->pixel_buffer);
  }

  if (grid_context_is_within_grid_context(e->canvas->grid, x, y)) {
    pixel_buffer_copy(e->canvas->pixel_buffer,
                      e->sprite_sheet->tiles[e->sprite_sheet->selected_sprite]);
  }
}

void sprite_editor_on_mouse_up(sprite_editor e, int x, int y) {}

void sprite_editor_update(void) {}

void sprite_editor_render(sprite_editor e, graphics g) {
#if sprite_sheet_ENABLED
  sprite_sheet_render(e->sprite_sheet, g);
#endif

  canvas_render(e->canvas);
  canvas_render(e->color_picker);

#if 0
  grid_context_render(e->toolbar);
#endif
}

void sprite_editor_on_mouse_down_left(sprite_editor e, int x, int y) {
  handle_mouse_click(e, x, y);
}

void sprite_editor_on_mouse_move(sprite_editor e, int x, int y) {
  if (!input_is_mouse_down())
    return;
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
