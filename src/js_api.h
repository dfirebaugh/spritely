/**
 * JS_API
 * Functions that will be exposed to javascript
 * 
 */

#ifndef _JS_API
#define _JS_API
#include "entity.h"

extern void print_something();
extern void render_char(char charcode, uint32_t x, uint32_t y);
extern void add_sprite_to_entity_manager(Entity_manager_t manager, uint8_t sprite_index, uint32_t x, uint32_t y);
extern void change_state(uint8_t new_state);
extern Entity_manager_t new_entity_manager();

#endif
