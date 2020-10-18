#include "globals.h"

void render_char(char charcode, uint32_t x, uint32_t y)
{
    // Sprite_sheet_render_sprite(main_font_sprite_sheet, charcode, x, y);
    Entity_manager_new_entity(spritely_entities, main_font_sprite_sheet, charcode, x, y);
}

void print_something()
{
    printf("something\n");
}

void change_state(uint8_t new_state)
{
  App_State_set_state(spritely_state, new_state);
}

Entity_manager_t new_entity_manager()
{
  Entity_manager_t new_entities = Entity_manager_make();

  return new_entities;
}

void add_sprite_to_entity_manager(Entity_manager_t manager, uint8_t sprite_index, uint32_t x, uint32_t y)
{
  Entity_manager_new_entity(manager, main_font_sprite_sheet, sprite_index, x, y);
}

