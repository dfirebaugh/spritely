
#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

#define COLOR_PICKER_NUM_COLORS 16

// 4 because RGBA
#define NUM_COLOR_COMPONENTS 4
#define COLOR_VALUES_SIZE COLOR_PICKER_NUM_COLORS *NUM_COLOR_COMPONENTS

typedef struct {
  uint8_t r, g, b, a;
} RGBA;

/* Default color palette, will be overridden when loading palette file */
typedef enum color {
  BACKGROUND = -1, /* default background color (black) */
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
} color;

// A one-dimensional array holding RGBA components for all color_t values
// e.g. color_components[color_t value] == Red component of that color
//  immediately followed by green, blue, and alpha component values
extern uint8_t color_values[COLOR_VALUES_SIZE];
extern RGBA color_to_rgba(color c);
extern color rgba_to_color(RGBA rgba);

#endif // COLORS_H
