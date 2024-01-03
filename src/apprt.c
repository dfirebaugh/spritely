#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "apprt.h"
#include "graphics.h"
#include "input.h"
#include "sprite_editor.h"
#include "state.h"

#define DEFAULT_SCREEN_WIDTH 240
#define DEFAULT_SCREEN_HEIGHT 160

struct app_runtime {
  graphics graphics;
  input input;
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
  if (a->graphics != NULL) {
    graphics_destroy(a->graphics);
    a->graphics = NULL;
  }
  if (a->editor != NULL) {
    sprite_editor_destroy(a->editor);
    a->editor = NULL;
  }
  free(a->input);
  free(a);

  printf("spritely has exited!\n");
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
    sprite_editor_render(a->editor);
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
