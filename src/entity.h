#ifndef _Entity
#define _Entity
#include "sprite_sheet.h"

typedef struct Entity_manager *Entity_manager_t;

extern Entity_manager_t Entity_manager_make();
extern void Entity_free(Entity_manager_t entity_manager);
extern void Entity_render(Entity_manager_t entity_manager);
extern void Entity_manager_new_entity(Entity_manager_t manager, Sprite_sheet_t sprite_sheet, uint8_t sprite_index, uint8_t x, uint8_t y);

#endif
