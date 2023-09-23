

#ifndef _COLORS_
#define _COLORS_

#include <stdint.h>
#include "defs.h"

/* Default color palette, will be overridden when loading palette file */
typedef enum color {
  BACKGROUND = -1, /* default background color (black color) */
  BLACK = 0,
  GREY1,
  GREY2,
  GREY4,
  GREY8,
  GREY16,
  D_BLUE,
  B_BLUE,
  B_RED,
  BLUE,
  GREEN,
  RED,
  CYAN,
  YELLOW,
  MAGENTA,
  WHITE
} color_t;

// A one-dimensional array holding RGBA components for all color_t values
// e.g. color_components[color_t value] == Red component of that color
//  immediately followed by green, blue, and alpha component values
extern uint8_t color_values[COLOR_VALUES_SIZE];

#endif
