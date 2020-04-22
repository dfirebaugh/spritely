#include "file.h"

static void print_sprite_as_hex(FILE *output, const char sprite_index)
{
    char i;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
    {
        if (i == 0)
        {
            fprintf(output,"{ 0x");
        }
        else if (i % 2 == 0)
        {
            fprintf(output,", 0x");
        }

        fprintf(output, "%x", sprite_sheet_cells[sprite_index].pixels[i]);

        if (i == SPRITE_CANVAS_SIZE-1)
            fprintf(output, " }\n");

    }

    fprintf(output, "\n");
}

static void print_sprite_sheet(FILE *output)
{
    char i;
    for (i = 0; i < SPRITESHEET_CANVAS_SIZE; i++)
        print_sprite_as_hex(output, i);
}

extern void save_file()
{
    FILE *fp;

    fp = fopen("_spritesheet.txt", "w+");
    print_sprite_sheet(fp);
    fclose(fp);
}
