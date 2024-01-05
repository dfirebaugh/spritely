#ifndef SPRITE_EDITOR_H
#define SPRITE_EDITOR_H

#include "canvas.h"
#include "graphics.h"
#include "grid_context.h"
#include "sprite_sheet.h"

typedef struct {
  canvas color_picker;
  canvas canvas;
  sprite_sheet sprite_sheet;
  color current_color;
  int selected_sprite;
} *sprite_editor;

extern sprite_editor sprite_editor_create(graphics g);
extern void sprite_editor_destroy(sprite_editor e);
extern void sprite_editor_render(sprite_editor e, graphics g);
extern void sprite_editor_on_mouse_down_right(sprite_editor e, int x, int y);
extern void sprite_editor_on_mouse_down_left(sprite_editor e, int x, int y);
extern void sprite_editor_on_mouse_up_right(sprite_editor e, int x, int y);
extern void sprite_editor_on_mouse_up_left(sprite_editor e, int x, int y);
extern void sprite_editor_on_mouse_move(sprite_editor e, int x, int y);

#endif // SPRITE_EDITOR_H
