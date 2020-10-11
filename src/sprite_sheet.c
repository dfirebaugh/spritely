#include "globals.h"

struct Sprite_sheet
{
    const char *filename;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect clips[SPRITESHEET_SIZE];
};

Sprite_sheet_t Sprite_sheet_make(const char *sprite_sheet)
{
    Sprite_sheet_t spr_sheet = malloc(sizeof(struct Sprite_sheet));
    spr_sheet->surface = IMG_Load(sprite_sheet); 
    spr_sheet->texture = SDL_CreateTextureFromSurface(renderer, spr_sheet->surface);

    //Setup the clips for our image
    for (int i = 0; i < SPRITESHEET_SIZE; ++i){
        spr_sheet->clips[i].x = (i % SPRITESHEET_ROW_SIZE) * SPRITE_WIDTH;
        spr_sheet->clips[i].y = (i / SPRITESHEET_ROW_SIZE) * SPRITE_HEIGHT;
        spr_sheet->clips[i].w = SPRITE_WIDTH;
        spr_sheet->clips[i].h = SPRITE_HEIGHT;
    }

    return spr_sheet;
}

void Sprite_sheet_free(Sprite_sheet_t spr_sheet)
{
    SDL_FreeSurface(spr_sheet->surface);
    free(spr_sheet);
}

void Sprite_sheet_render_sprite(Sprite_sheet_t spr_sheet, const unsigned int sprite_index, unsigned int x, unsigned int y)
{
    SDL_Surface* window_surface = SDL_GetWindowSurface(window);
    SDL_Rect destination_rect;

    destination_rect.x = x;
    destination_rect.y = y;
    destination_rect.w = 25;
    destination_rect.h = 25;

    SDL_RenderCopy(renderer, spr_sheet->texture, &spr_sheet->clips[sprite_index], &destination_rect);
}
