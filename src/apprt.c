#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "apprt.h"
#include "graphics.h"
#include "input.h"
#include "pixel_buffer.h"
#include "sprite_editor.h"
#include "state.h"

#define DEFAULT_SCREEN_WIDTH 240
#define DEFAULT_SCREEN_HEIGHT 160

struct app_runtime {
  graphics graphics;
  sprite_editor editor;
};

app_runtime app_runtime_init(void) {
  app_runtime a = malloc(sizeof(struct app_runtime));
  if (!a)
    return NULL;

  graphics g =
      graphics_init(DEFAULT_SCREEN_WIDTH * 4, DEFAULT_SCREEN_HEIGHT * 4);
  if (!g)
    return NULL;

  state_init();

  a->graphics = g;
  a->editor = sprite_editor_create(g);

  state_set(SPRITE_EDITOR);
  return a;
}

void app_runtime_destroy_and_exit(app_runtime a) {
  if (!a)
    return;
  if (a->graphics) {
    graphics_destroy(a->graphics);
    a->graphics = NULL;
  }
  if (a->editor) {
    sprite_editor_destroy(a->editor);
    a->editor = NULL;
  }
  free(a);
  exit(0);
}

bool app_runtime_should_exit(app_runtime a) {
  return state_get() == SHOULD_EXIT;
}

void app_runtime_run(app_runtime a) {
  if (do_polling(a)) {
    state_set(SHOULD_EXIT);
  }

  switch (state_get()) {
  case SHOULD_EXIT:
    app_runtime_destroy_and_exit(a);
    break;
  case RUNNING:
    break;
  case SPRITE_EDITOR:
    graphics_set_draw_color(a->graphics, 74, 50, 110, 255);
    graphics_clear(a->graphics);
    sprite_editor_render(a->editor, a->graphics);
    graphics_render_present(a->graphics);
    break;
  default:
    break;
  }
}

void on_mouse_down_right(app_runtime a, int x, int y) {
  sprite_editor_on_mouse_down_right(a->editor, x, y);
}
void on_mouse_down_left(app_runtime a, int x, int y) {
  sprite_editor_on_mouse_down_left(a->editor, x, y);
}

void on_mouse_up_right(app_runtime a, int x, int y) {
  sprite_editor_on_mouse_up_right(a->editor, x, y);
}

void on_mouse_up_left(app_runtime a, int x, int y) {
  sprite_editor_on_mouse_up_left(a->editor, x, y);
}

void on_mouse_move(app_runtime a, int x, int y) {
  sprite_editor_on_mouse_move(a->editor, x, y);
}

void copy_buffer_to_canvas(app_runtime a) {
  pixel_buffer_copy(
      a->editor->sprite_sheet->tiles[a->editor->sprite_sheet->selected_sprite],
      a->editor->canvas->pixel_buffer);
}

void copy_sprite_to_buffer(app_runtime a) {
  sprite_sheet sp = a->editor->sprite_sheet;
  pixel_buffer_copy(sp->tiles[sp->selected_sprite], sp->copy_buffer);
}

void paste_sprite_from_buffer(app_runtime a) {
  sprite_sheet sp = a->editor->sprite_sheet;
  pixel_buffer_copy(sp->copy_buffer, sp->tiles[sp->selected_sprite]);
  copy_buffer_to_canvas(a);
}

void copy_sprite_to_canvas(app_runtime a) {}
