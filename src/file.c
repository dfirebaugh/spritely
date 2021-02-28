#include "defs.h"
#include "sfd.h"
#include "util.h"
#include "sprite_editor.h"
#include <stdint.h>
#include "message_queue.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/**
  8: palette_info_header
  48: COLORPICKER_NUM_COLORS*3
  1 : checksum
*/
#define PALETTE_INFO_LEN ((COLORPICKER_NUM_COLORS*3)+8+1)
static const char* palette_info_header = "SPRITELY";

static void append_color_palette(const char* filepath);
static void load_color_palette(const char* filepath);

// Populate the file_data array with the row-th row of sprites
// row is assumed to be in range [0, SPRITESHEET_COL_SIZE)
static void populate_file_data_with_spritesheet_row(uint8_t *const file_data, const int row) {
    const int sprite_start = row * SPRITESHEET_ROW_SIZE;
    int file_data_index = row * SPRITESHEET_ROW_SIZE * SPRITE_CANVAS_SIZE * NUM_COLOR_COMPONENTS;

    // First we output the top row of pixels of each sprite in the row,
    //  then the second row of pixels of each sprite and so on

    // Iterate pixel rows of the sprite canvases
    for (int r = 0; r < SPRITE_CANVAS_ROW_SIZE; r += 1) {
        const int pixel_start = r * SPRITE_CANVAS_ROW_SIZE;

        // Iterate sprites in the row
        for (int s = sprite_start; s < sprite_start + SPRITESHEET_ROW_SIZE; s += 1) {
            Context_t sprite = sprite_selector_cells[s];

            // Iterate pixels in the sprite canvas row
            for (int p = pixel_start; p < pixel_start + SPRITE_CANVAS_ROW_SIZE; p += 1) {
                const size_t base = Context_get_pixel(sprite, p) * NUM_COLOR_COMPONENTS;

                for (int c = 0; c < NUM_COLOR_COMPONENTS; c += 1) {
                    file_data[file_data_index] = color_values[base + c];
                    file_data_index += 1;
                }
            }
        }
    }
}

// Save the the spritesheet to a file having filename
static void save_spritesheet(const char *const filename) {
    uint8_t file_data[NUM_COLOR_COMPONENTS * SPRITESHEET_SIZE * SPRITE_CANVAS_SIZE];

    // Populate file_data
    // The size of a column is the number of rows there are
    for (int r = 0; r < SPRITESHEET_COL_SIZE; r += 1) {
        populate_file_data_with_spritesheet_row(file_data, r);
    }

    const int write_code = stbi_write_png(
        filename,
        SPRITE_CANVAS_ROW_SIZE * SPRITESHEET_ROW_SIZE, // image width
        SPRITE_CANVAS_ROW_SIZE * SPRITESHEET_COL_SIZE, // image height
        NUM_COLOR_COMPONENTS,
        file_data,
        SPRITE_CANVAS_ROW_SIZE * SPRITESHEET_ROW_SIZE * NUM_COLOR_COMPONENTS // stride in bytes
    );

    if (write_code == 0) {
        Message_Queue_enqueue(command_message_queue, "Failed to save spritesheet", 0);
        return;
    }
    append_color_palette(filename);
    Message_Queue_enqueue(command_message_queue, "Spritesheet saved", 0);
}

static void save_sprites(
    const char *const filename_prefix,// Not necessarily nul terminated
    const int filename_prefix_len)
{
    // 20 is long enough to hold the additional "_(XX,YY).png"
    const size_t filename_buf_len = filename_prefix_len + 20;
    char *const filename_buf = checked_malloc(sizeof(char) * filename_buf_len);

    memcpy(filename_buf, filename_prefix, filename_prefix_len);

    for (int y = 0; y < SPRITESHEET_COL_SIZE; y += 1) {
        for (int x = 0; x < SPRITESHEET_ROW_SIZE; x += 1) {
            const size_t sprite_index = x + y * SPRITESHEET_ROW_SIZE;
            Context_t sprite = sprite_selector_cells[sprite_index];

            if (Context_is_solid_color(sprite, BACKGROUND)) {
                continue;// Do not output blank (background color only) sprite cells
            }

            sprintf(&filename_buf[filename_prefix_len], "_(%.2d,%.2d).png", x, y);

            uint8_t file_data[SPRITE_CANVAS_SIZE * NUM_COLOR_COMPONENTS];
            size_t file_data_index = 0;

            // Populate the file_data buffer with the sprite
            for (int i = 0; i < SPRITE_CANVAS_SIZE; i += 1) {
                const size_t base = Context_get_pixel(sprite, i) * NUM_COLOR_COMPONENTS;

                for (int c = 0; c < NUM_COLOR_COMPONENTS; c += 1) {
                    file_data[file_data_index] = color_values[base + c];
                    file_data_index += 1;
                }
            }

            const int write_code = stbi_write_png(
                filename_buf,
                SPRITE_CANVAS_ROW_SIZE, // image width
                SPRITE_CANVAS_ROW_SIZE, // image height
                NUM_COLOR_COMPONENTS,
                file_data,
                SPRITE_CANVAS_ROW_SIZE * NUM_COLOR_COMPONENTS // stride in bytes
            );

            if (write_code == 0) {
                Message_Queue_enqueue(command_message_queue, "Error outputting sprites", 0);
                return;// Do not attempt to output any more images
            }

            append_color_palette(filename_buf);
        }
    }

    free(filename_buf);
}

void save_file(int save_individual_sprites) {
#ifdef __EMSCRIPTEN__
    return;
#else
    // Populate the options struct for the save dialog
    sfd_Options opt = {
        .filter_name  = "Image File",
        .filter       = "*.png"
    };

    if (save_individual_sprites) {
        opt.title = "Save Spritesheet and Sprites";
    }
    else {
        opt.title = "Save Spritesheet";
    }

    // Open the save dialog and get filename
    const char *filename = sfd_save_dialog(&opt);

    if (filename == NULL) {
        // Save dialog was canceled or closed
        Message_Queue_enqueue(command_message_queue, "Saving was canceled", 0);
        return;
    }

    save_spritesheet(filename);

    if (save_individual_sprites) {
        const size_t filename_prefix_len = find_last_occurrence(filename, strlen(filename), '.');
        save_sprites(filename, filename_prefix_len);
    }
#endif
}

static void populate_spritesheet_row_with_file_data(const uint8_t *const file_data, const int row) {
    const int sprite_start = row * SPRITESHEET_ROW_SIZE;
    int file_data_index = row * SPRITESHEET_ROW_SIZE * SPRITE_CANVAS_SIZE * NUM_COLOR_COMPONENTS;

    // Iterate pixel rows of the sprite canvases
    for (int r = 0; r < SPRITE_CANVAS_ROW_SIZE; r += 1) {
        const int pixel_start = r * SPRITE_CANVAS_ROW_SIZE;

        // Iterate sprites in the row
        for (int s = sprite_start; s < sprite_start + SPRITESHEET_ROW_SIZE; s += 1) {
            Context_t sprite = sprite_selector_cells[s];

            // Iterate pixels in the sprite canvas row
            for (int p = pixel_start; p < pixel_start + SPRITE_CANVAS_ROW_SIZE; p += 1) {
                const uint8_t r = file_data[file_data_index];
                file_data_index += 1;
                const uint8_t g = file_data[file_data_index];
                file_data_index += 1;
                const uint8_t b = file_data[file_data_index];
                file_data_index += 1;
                const uint8_t a = file_data[file_data_index];
                file_data_index += 1;

                color_t color = get_pixel_render_color(r, g, b, a);
                Context_set_pixel(sprite, p, color);
            }
        }
    }
}

void open_file() {
#ifdef __EMSCRIPTEN__
    return;
#else
    // Populate the options struct for the open dialog
    sfd_Options opt = {
        .filter_name  = "Image File",
        .filter       = "*.png",
        .title        = "Open Spritesheet"
    };

    // Open the dialog and get filename
    const char *filename = sfd_open_dialog(&opt);

    if (filename == NULL) {
        // Open dialog was canceled or closed
        Message_Queue_enqueue(command_message_queue, "Opening was canceled", 0);
        return;
    }

    int image_width;
    int image_height;
    int num_channels; // What the image file actually has. Not useful to know.

    // Open given file and populate sprites using the file's image data

    // We are guaranteed to get NUM_COLOR_COMPONENTS number of channels in the result if successful
    const unsigned char *const image_data = stbi_load(filename, &image_width, &image_height, &num_channels, NUM_COLOR_COMPONENTS);

    if (image_data == NULL) {
        // "The function stbi_failure_reason() can be queried for an extremely brief, end-user
        //  unfriendly explanation of why the load failed." via stb_image.h
        Message_Queue_enqueue(command_message_queue, "Allocation failure or image is corrupt or invalid", 0);
        return;
    }

    const int desired_width  = SPRITESHEET_ROW_SIZE * SPRITE_CANVAS_ROW_SIZE;
    const int desired_height = SPRITESHEET_COL_SIZE * SPRITE_CANVAS_ROW_SIZE;

    if (image_width != desired_width || image_height != desired_height) {
        Message_Queue_enqueue(command_message_queue, "Given spritesheet has incorrect dimensions", 0);
        return;
    }

    load_color_palette(filename);

    for (int i = 0; i < SPRITESHEET_COL_SIZE; i += 1) {
        populate_spritesheet_row_with_file_data(image_data, i);
    }
#endif
}

static void append_color_palette(const char* filepath)
{
    FILE* file = fopen(filepath, "a");

    if(file != NULL)
    {
        fwrite(palette_info_header, strlen(palette_info_header), 1, file);
        const int palette_size = COLORPICKER_NUM_COLORS;
        int color_idx = 0;
        uint8_t cs = 0;

        for(int i = 0; i < palette_size && !feof(file); ++i)
        {
            uint8_t r = color_values[color_idx++];
            uint8_t g = color_values[color_idx++];
            uint8_t b = color_values[color_idx++];
            color_idx++;
            cs ^= r;
            cs ^= g;
            cs ^= b;
            fwrite(&r, 1, 1, file);
            fwrite(&g, 1, 1, file);
            fwrite(&b, 1, 1, file);
        }
        fwrite(&cs, 1, 1, file);
        fclose(file);
    }
}

static void load_color_palette(const char* filepath)
{
    FILE* file = fopen(filepath, "rb");

    if(file != NULL)
    {
        int file_len = 0;

        fseek(file, 0, SEEK_END);
        file_len = ftell(file);

        if(file_len > PALETTE_INFO_LEN)
        {
            const int palette_size = COLORPICKER_NUM_COLORS;
            const int header_len = strlen(palette_info_header);
            uint8_t buf[PALETTE_INFO_LEN];

            fseek(file, -(PALETTE_INFO_LEN), SEEK_END);
            fread(buf, PALETTE_INFO_LEN, 1, file);

            if(memcmp(buf, palette_info_header, header_len) == 0)
            {
                uint8_t cs = 0;
                uint8_t *data = &buf[header_len];

                for(int i = 0; i < palette_size * 3; ++i)
                {
                    cs ^= data[i];
                }

                if(cs == data[palette_size * 3])
                {
                    for(int i = 0; i < palette_size; ++i)
                    {
                        color_values[i*4]   = data[i*3];
                        color_values[i*4+1] = data[i*3+1];
                        color_values[i*4+2] = data[i*3+2];
                        color_values[i*4+3] = 255;
                    }
                    Context_render(color_picker_ctx);
                }
            }
        }

        fclose(file);
    }
}