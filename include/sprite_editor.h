#ifndef SPRITE_EDITOR_H
#define SPRITE_EDITOR_H

#include "canvas.h"
#include "graphics.h"
#include "grid_context.h"
#include "input.h"
#include "sprite_picker.h"

typedef struct {
  graphics graphics;
  input input;
  canvas color_picker;
  canvas canvas;
  sprite_picker sprite_picker;
  grid_context toolbar;
  color current_color;
  int selected_sprite;
} *sprite_editor;

extern sprite_editor sprite_editor_create(graphics g);
extern void sprite_editor_destroy(sprite_editor e);
extern void sprite_editor_render(sprite_editor e);
extern void sprite_editor_on_mouse_down_right(sprite_editor e, int x, int y);
extern void sprite_editor_on_mouse_down_left(sprite_editor e, int x, int y);
extern void sprite_editor_on_mouse_up_right(sprite_editor e, int x, int y);
extern void sprite_editor_on_mouse_up_left(sprite_editor e, int x, int y);
extern void sprite_editor_on_mouse_move(sprite_editor e, int x, int y);

#endif // SPRITE_EDITOR_H
