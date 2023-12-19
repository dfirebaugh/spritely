
#include "grid_context.h"
#include "pixel_buffer.h"

void editor_tool_set_current_color(pixel_buffer pb, coordinate local_coord,
                                   color *current_color) {
  RGBA rgba = pixel_buffer_get_rgba(pb, local_coord.x, local_coord.y);
  *current_color = rgba_to_color(rgba);
}

void editor_tool_draw_to_canvas(pixel_buffer pb, coordinate local_coord,
                                color *current_color) {
#if 0
  sprite_editor e = current_editor_context;
#endif

  RGBA c = color_to_rgba(*current_color);
  pixel_buffer_set_pixel(pb, local_coord.x, local_coord.y, c);
}

void editor_tool_select_sprite(pixel_buffer pb, coordinate local_coord,
                               color *current_color) {
#if 0
  sprite_editor e = current_editor_context;

  pixel_buffer_copy(e->sprite_picker->pixel_buffer, e->canvas->pixel_buffer,
                    e->selected_sprite);
#endif
}
