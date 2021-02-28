/**
 * JS_API
 * Functions that will be exposed to javascript
 * 
 */

#ifndef _JS_API
#define _JS_API
#include "entity.h"
#include <stdint.h>
#include "globals.h"

extern void print_something();
extern void render_char(char charcode, uint32_t x, uint32_t y);
extern void add_sprite_to_entity_manager(Entity_manager_t manager, uint8_t sprite_index, uint32_t x, uint32_t y);
extern void change_state(uint8_t new_state);
extern Entity_manager_t new_entity_manager();
extern void render_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color);
extern void render_rect_fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color);
extern void set_render_color(uint8_t color);

extern void register_draw_fn(void (*f)(void));
extern void register_update_fn(void (*f)(void));
extern void clear();

extern bool is_btn_pressed(uint8_t btn);
extern void game_inputs(SDL_Event event);

#if 0
extern void circle(uint8_t x, uint8_t y, uint8_t radius);
extern void circle_fill(uint8_t x, uint8_t y, uint8_t radius, uint8_t color);
#endif

#endif