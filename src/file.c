#include "file.h"

static void print_sprite(FILE *output, const char sprite_index)
{
    char i;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
    {
        if (i % 2 == 0)
            printf(" ");

        if (i % 8 == 0)
            fprintf(output, "\n");
        fprintf(output, "%x", sprite_sheet_cells[sprite_index].pixels[i]);
    }

    fprintf(output, "\n");
}

static void print_sprite_sheet(FILE *output)
{
    char i;
    for (i = 0; i < SPRITESHEET_CANVAS_SIZE; i++)
        print_sprite(output, i);
}

extern void save_file()
{
    FILE *fp;

    fp = fopen("spritesheet.txt", "w+");
    print_sprite_sheet(fp);
    fclose(fp);
}
