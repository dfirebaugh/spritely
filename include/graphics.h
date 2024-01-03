
#ifndef SPRITELY_GRAPHICS_H
#define SPRITELY_GRAPHICS_H

#include "color.h"
#include <stdint.h>

typedef struct graphics *graphics;

typedef struct Rect {
  int r, g, b, a;
  int x, y;
  int w, h;
} Rect;

extern graphics graphics_init(int screen_width, int screen_height);
extern void graphics_draw_rect(graphics g, Rect *rect);
extern void graphics_fill_rect(graphics g, Rect *rect);
extern void graphics_destroy(graphics gfx);
extern void graphics_clear(graphics g);
extern void graphics_render_present(graphics g);
extern int graphics_get_screen_width(graphics g);
extern int graphics_get_screen_height(graphics g);
extern void graphics_set_draw_color(graphics gfx, uint8_t r, uint8_t g,
                                    uint8_t b, uint8_t a);

#endif // SPRITELY_GRAPHICS_H
