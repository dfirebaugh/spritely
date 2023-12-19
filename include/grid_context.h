#ifndef grid_context_H
#define grid_context_H

#include <stdbool.h>
#include <stdint.h>

#include "graphics.h"
#include "pixel_buffer.h"

typedef struct element {
  void (*draw)(struct element *);
} *grid_context_element;

typedef struct {
  float x;
  float y;
} offset;

typedef struct {
  int x;
  int y;
} coordinate;

typedef struct {
  graphics graphics;
  pixel_buffer pixel_buffer;
  coordinate selected;
  offset offset;
  bool has_indicator;
  int total_elements;
  int row_count;
  int col_count;
  int offset_x;
  int offset_y;
  int scale_factor;
  int grid_gap;
  int col_gap;
  int row_gap;
} *grid_context;

typedef enum { GRAPHIC_TYPE_IMAGE, GRAPHIC_TYPE_COLOR } graphic_type;

typedef union {
  uint32_t color;
} graphic_union;

typedef void (*mouse_down_callback)(pixel_buffer pb, coordinate local_coord,
                                    color *current_color);

extern grid_context grid_context_create(graphics gfx, int col_count,
                                        int row_count, int scale_factor,
                                        int offset_x, int offset_y);
extern void grid_context_destroy(grid_context w);
extern void grid_context_render(grid_context w);
extern void grid_enable_indicator(grid_context w);
extern void grid_disable_indicator(grid_context w);
extern void grid_context_on_mouse_down(grid_context w, pixel_buffer pb,
                                       float click_x, float click_y,
                                       color *current_color,
                                       mouse_down_callback callback);

#endif // grid_context_H
