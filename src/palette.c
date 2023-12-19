#include <stdbool.h>
#include <stdio.h>

#include "color.h"

bool load_palette(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Failed to open the file");
    return false;
  }

  for (int i = 0; i < COLOR_PICKER_NUM_COLORS; ++i) {
    int r, g, b;
    if (fscanf(file, "%d,%d,%d", &r, &g, &b) != 3) {
      printf("Failed to read the RGB values for color %d\n", i + 1);
      fclose(file);
      return false;
    }
    int index = i * NUM_COLOR_COMPONENTS;
    color_values[index] = r;
    color_values[index + 1] = g;
    color_values[index + 2] = b;
    color_values[index + 3] = 255;
  }

  fclose(file);
  return true;
}
