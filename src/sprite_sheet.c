#include "sprite_sheet.h"

void print_sprite(const char sprite_sheet_index)
{
    int i;
    for(i = 0; i < SPRITE_CANVAS_SIZE; i++)
        printf("%x", sprite_sheet[sprite_sheet_index][i]);

    printf("\n");
}

void push_to_sprite_sheet(const char sprite_sheet_index, const char *spr)
{
    char i;
    for(i = 0; i < SPRITE_CANVAS_SIZE; i++)
        sprite_sheet[sprite_sheet_index][i] = spr[i];
    print_sprite(sprite_sheet_index);
}

uint *get_sprite(const char index)
{
    print_sprite(index);
    return sprite_sheet[index];
}
