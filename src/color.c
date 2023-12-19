#include <stdint.h>

#include "color.h"

/* Default color palette, will be overridden when loading palette file */
uint8_t color_values[COLOR_VALUES_SIZE] = {
    0,   0,   0,   255, // BLACK
    224, 224, 224, 255, // GREY1
    192, 192, 192, 255, // GREY2
    160, 160, 160, 255, // GREY4
    96,  96,  96,  255, // GREY8
    32,  32,  32,  255, // GREY16
    0,   0,   100, 255, // D_BLUE
    0,   0,   255, 255, // B_BLUE
    255, 0,   0,   255, // B_RED
    0,   0,   200, 255, // BLUE
    0,   200, 0,   255, // GREEN
    200, 0,   0,   255, // RED
    0,   255, 255, 255, // CYAN
    255, 255, 0,   255, // YELLOW
    255, 0,   255, 255, // MAGENTA
    255, 255, 255, 255, // WHITE
};

RGBA color_to_rgba(color c) {
  RGBA result;

  if (c == BACKGROUND) {
    result.r = 0;
    result.g = 0;
    result.b = 0;
    result.a = 255;
    return result;
  }

  int index = c * NUM_COLOR_COMPONENTS;

  result.r = color_values[index];
  result.g = color_values[index + 1];
  result.b = color_values[index + 2];
  result.a = color_values[index + 3];

  return result;
}

color rgba_to_color(RGBA rgba) {
  int min_distance_sq = INT32_MAX;
  int best_match = -1;

  for (int i = 0; i < COLOR_VALUES_SIZE; i += NUM_COLOR_COMPONENTS) {
    int dr = rgba.r - color_values[i];
    int dg = rgba.g - color_values[i + 1];
    int db = rgba.b - color_values[i + 2];
    int da = rgba.a - color_values[i + 3];

    int distance_sq = dr * dr + dg * dg + db * db + da * da;

    if (distance_sq < min_distance_sq) {
      min_distance_sq = distance_sq;
      best_match = i;
    }
  }

  // If no close match was found, we return BACKGROUND.
  if (best_match == -1)
    return BACKGROUND;

  return (color)(best_match / NUM_COLOR_COMPONENTS);
}
