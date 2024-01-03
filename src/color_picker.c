#include <stdio.h>

#include "canvas.h"
#include "color.h"
#include "palette.h"

canvas color_picker_create(graphics g) {
  if (!load_palette("./assets/palette/palette.cfg")) {
    printf("Failed to load palette!\n");
  }
  canvas cp = canvas_create(g, 4, 4, 44, 600, 64);
  grid_enable_indicator(cp->grid);

  for (int index = 0; index < COLOR_PICKER_NUM_COLORS; index++) {
    int x = index % 4;
    int y = index / 4;

    color col_enum_value = (x + y * 4) % COLOR_PICKER_NUM_COLORS;
    pixel_buffer_set_pixel(cp->pixel_buffer, x, y,
                           color_to_rgba(col_enum_value));
  }

  return cp;
}
