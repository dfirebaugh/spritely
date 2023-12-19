
#ifndef EDITOR_TOOLS_H
#define EDITOR_TOOLS_H

#include "grid_context.h"
#include "pixel_buffer.h"

extern void editor_tool_set_current_color(pixel_buffer pb,
                                          coordinate local_coord,
                                          color *current_color);
extern void editor_tool_draw_to_canvas(pixel_buffer pb, coordinate local_coord,
                                       color *current_color);
extern void editor_tool_select_sprite(pixel_buffer pb, coordinate local_coord,
                                      color *current_color);

#endif // EDITOR_TOOLS_H
