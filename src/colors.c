#include "colors.h"
/* Default color palette, will be overridden when loading palette file */
uint8_t color_values[COLOR_VALUES_SIZE] = {
      0,   0,   0, 255, // BLACK
    224, 224, 224, 255, // GREY1
    192, 192, 192, 255, // GREY2
    160, 160, 160, 255, // GREY4
     96,  96,  96, 255, // GREY8
     32,  32,  32, 255, // GREY16
      0,   0, 100, 255, // D_BLUE
      0,   0, 255, 255, // B_BLUE
    255,   0,   0, 255, // B_RED
      0,   0, 200, 255, // BLUE
      0, 200,   0, 255, // GREEN
    200,   0,   0, 255, // RED
      0, 255, 255, 255, // CYAN
    255, 255,   0, 255, // YELLOW
    255,   0, 255, 255, // MAGENTA
    255, 255, 255, 255, // WHITE
};
