#ifndef _SPRITESHEET
#define _SPRITESHEET

typedef struct Sprite_sheet *Sprite_sheet_t;

extern Sprite_sheet_t Sprite_sheet_make(const char *sprite_sheet);
extern void Sprite_sheet_free(Sprite_sheet_t spr_sheet);

/**
 * Sprite_sheet_render_sprite
 * render a specific sprite from a sprite sheet at a specific location (x, y) 
 */
extern void Sprite_sheet_render_sprite(Sprite_sheet_t spr_sheet, const unsigned int sprite_index, unsigned int x, unsigned int y);
extern void Sprite_sheet_render_sprite_scale(Sprite_sheet_t spr_sheet, const unsigned int sprite_index, unsigned int x, unsigned int y, float scale);
#endif
