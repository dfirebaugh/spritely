#include "globals.h"
#include <stdlib.h>

void set_pixel_render_color(color_t p)
{
    const size_t base = p * NUM_COLOR_COMPONENTS;
    SDL_SetRenderDrawColor(renderer,
        color_values[base + 0],
        color_values[base + 1],
        color_values[base + 2],
        color_values[base + 3]
    );
}

color_t get_pixel_render_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
    for (color_t color = 0; color < COLORPICKER_NUM_COLORS; color += 1) {
        const size_t base = color * NUM_COLOR_COMPONENTS;

        const int r_matches = color_values[base + 0] == r;
        const int g_matches = color_values[base + 1] == g;
        const int b_matches = color_values[base + 2] == b;
        const int a_matches = color_values[base + 3] == a;

        if (r_matches && g_matches && b_matches && a_matches) {
            return color;
        }
    }

    // Return black if no exact match found
    return BLACK;
}

void *checked_malloc(size_t size) {
    void *const ptr = malloc(size);

    if (size != 0 && ptr == NULL) {
        fprintf(
            stderr,"Ran out of memory. Failed to malloc %zu bytes.\n", size);
        exit(1);
    }

    return ptr;
}

size_t find_last_occurrence(const char *const buf, const size_t buf_len, const char ch) {
    for (size_t i = buf_len - 1; i >= 0; i -= 1) {
        if (buf[i] == ch) {
            return i;
        }
    }

    return buf_len;
}

int sprite_sheet_index_in_range(const unsigned char index)
{
    if (index < 0 || index > SPRITESHEET_SIZE - 1) return 1;

    return 0;
}
