#include "globals.h"

uint sprite_sheet[SPRITESHEET_SIZE][SPRITE_CANVAS_SIZE];

void push_to_sprite_sheet(const char sprite_sheet_index, const char *spr);

uint *get_sprite(const char index);
