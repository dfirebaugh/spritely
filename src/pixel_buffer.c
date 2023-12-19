
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "graphics.h"
#include "pixel_buffer.h"

void pixel_buffer_set_pixel(pixel_buffer pb, int x, int y, RGBA c);
void pixel_buffer_get_size(pixel_buffer pb, int *width, int *height);

pixel_buffer pixel_buffer_create(int width, int height) {
  pixel_buffer pb = (pixel_buffer)malloc(sizeof(pixel_buffer));
  if (!pb)
    return NULL;

  pb->width = width;
  pb->height = height;
  pb->img = (RGBA **)malloc(height * sizeof(RGBA *));
  if (!pb->img)
    return NULL;

  for (int i = 0; i < height; i++) {
    pb->img[i] = (RGBA *)malloc(width * sizeof(RGBA));
    if (!pb->img[i])
      return NULL;
  }
  return pb;
}

void pixel_buffer_destroy(pixel_buffer pb) {
  for (int i = 0; i < pb->height; i++)
    free(pb->img[i]);

  free(pb->img);
  free(pb);
}

void pixel_buffer_set_pixel(pixel_buffer pb, int x, int y, RGBA c) {
  if (x >= 0 && x < pb->width && y >= 0 && y < pb->height) {
    pb->img[y][x] = c;
  }
}

RGBA pixel_buffer_get_rgba(pixel_buffer pb, int x, int y) {
  return pb->img[y][x];
}

void pixel_buffer_get_size(pixel_buffer pb, int *width, int *height) {
  *width = pb->width;
  *height = pb->height;
}

bool is_row_of_color(RGBA **img, int y, int width, RGBA c) {
  for (int x = 0; x < width; x++) {
    RGBA pixel = img[y][x];
    if (pixel.r != c.r || pixel.g != c.g || pixel.b != c.b || pixel.a != c.a) {
      return false;
    }
  }
  return true;
}

int pixel_buffer_get_width(pixel_buffer pb) {
  if (!pb) {
    return 0;
  }
  return pb->width;
}

int pixel_buffer_get_height(pixel_buffer pb) {
  if (!pb) {
    return 0;
  }
  return pb->height;
}

void pixel_buffer_fill(pixel_buffer pb, RGBA c) {
  for (int y = 0; y < pb->height; y++) {
    for (int x = 0; x < pb->width; x++) {
      pixel_buffer_set_pixel(pb, x, y, c);
    }
  }
}

void pixel_buffer_copy(pixel_buffer src, pixel_buffer dest, int sprite_index) {
  for (int y = 0; y < src->height; y++) {
    for (int x = 0; x < src->width; x++) {
      RGBA color = pixel_buffer_get_rgba(src, x, y);
      pixel_buffer_set_pixel(dest, x, y, color);
    }
  }
}

Rect *pixel_buffer_to_rects(pixel_buffer pb, int *num_rects, int scale_factor,
                            int offset_x, int offset_y) {
  *num_rects = 0;
  int max_rects = pb->height * pb->width;
  Rect *rects = (Rect *)malloc(max_rects * sizeof(Rect));
  if (!rects)
    return NULL;

  int i = 0;
  while (i < pb->height) {
    int j = 0;
    while (j < pb->width) {
      RGBA current_color = pb->img[i][j];

      // Skip transparent pixels
      if (current_color.a == 0) {
        j++;
        continue;
      }

      rects[*num_rects].r = current_color.r;
      rects[*num_rects].g = current_color.g;
      rects[*num_rects].b = current_color.b;
      rects[*num_rects].a = current_color.a;
      rects[*num_rects].y = i * scale_factor + offset_y;
      rects[*num_rects].x = j * scale_factor + offset_x;

      int k = j;
      while (k < pb->width && pb->img[i][k].r == current_color.r &&
             pb->img[i][k].g == current_color.g &&
             pb->img[i][k].b == current_color.b &&
             pb->img[i][k].a == current_color.a) {
        k++;
      }
      rects[*num_rects].w = (k - j) * scale_factor;

      int l = i + 1;
      while (l < pb->height &&
             is_row_of_color(pb->img, l, (k - j), current_color)) {
        l++;
      }
      rects[*num_rects].h = (l - i) * scale_factor;

      (*num_rects)++;
      j = k;
    }
    i++;
  }

  return rects;
}

void pixel_buffer_render(pixel_buffer pb, graphics gfx, int scale_factor,
                         int offset_x, int offset_y) {
  int nrects;
  Rect *color_picker_rects =
      pixel_buffer_to_rects(pb, &nrects, scale_factor, offset_x, offset_y);

  for (int i = 0; i < nrects; i++) {
    graphics_set_draw_color(gfx, color_picker_rects[i].r,
                            color_picker_rects[i].g, color_picker_rects[i].b,
                            color_picker_rects[i].a);
    graphics_fill_rect(gfx, &color_picker_rects[i]);
  }
  free(color_picker_rects);
}
