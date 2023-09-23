#include "app_state.h"
#include "colors.h"
#include "draw_tools.h"
#include "entity.h"
#include "globals.h"

App_State_t spritely_state;

Entity_manager_t spritely_entities;

void init_js_api(void) { spritely_entities = Entity_manager_make(); }

void render_char(char charcode, uint32_t x, uint32_t y) {
  // Sprite_sheet_render_sprite(main_font_sprite_sheet, charcode, x, y);
  // Entity_manager_new_entity(spritely_entities, main_font_sprite_sheet,
  // charcode, x, y);
}

void print_something(void) { printf("something\n"); }

void change_state(uint8_t new_state) {
  App_State_set_state(spritely_state, new_state);
}

Entity_manager_t new_entity_manager(void) {
  Entity_manager_t new_entities = Entity_manager_make();

  return new_entities;
}

void add_sprite_to_entity_manager(Entity_manager_t manager,
                                  uint8_t sprite_index, uint32_t x,
                                  uint32_t y) {
  //   Entity_manager_new_entity(spritely_entities, main_font_sprite_sheet,
  //   sprite_index, x, y);
}

void clear(void) { SDL_RenderClear(renderer); }

void register_draw_fn(void (*f)(void)) { js_draw = (*f); }

void register_update_fn(void (*f)(void)) { js_update = (*f); }

void render_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                 uint8_t color) {
  SDL_Rect new_rect;

  new_rect.x = x;
  new_rect.w = width;
  new_rect.h = height;
  new_rect.y = y;

  SDL_SetRenderDrawColor(renderer, color_values[color], color_values[color + 1],
                         color_values[color + 2], 255);
  SDL_RenderDrawRect(renderer, &new_rect);
}

void render_rect_fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                      uint8_t color) {
  SDL_Rect new_rect;

  new_rect.x = x;
  new_rect.w = width;
  new_rect.h = height;
  new_rect.y = y;

  SDL_SetRenderDrawColor(renderer, color_values[color], color_values[color + 1],
                         color_values[color + 2], 255);
  SDL_RenderFillRect(renderer, &new_rect);
}

void set_render_color(uint8_t color) {
  SDL_SetRenderDrawColor(renderer, color_values[color], color_values[color + 1],
                         color_values[color + 2], 255);
}

struct buttons {
  bool left;
  bool right;
  bool up;
  bool down;
  bool secondary;
  bool primary;
  bool pause;
} keydowns;

enum btn_enum {
  LEFT_BTN = 0,
  RIGHT_BTN,
  UP_BTN,
  DOWN_BTN,
  SECONDARY_BTN,
  PRIMARY_BTN,
  PAUSE_BTN
};

bool is_btn_pressed(uint8_t btn) {
  switch (btn) {
  case LEFT_BTN:
    return keydowns.left;
    break;
  case RIGHT_BTN:
    return keydowns.right;
    break;
  case UP_BTN:
    return keydowns.up;
    break;
  case DOWN_BTN:
    return keydowns.down;
    break;
  case SECONDARY_BTN:
    return keydowns.secondary;
    break;
  case PRIMARY_BTN:
    return keydowns.primary;
    break;
  case PAUSE_BTN:
    return keydowns.pause;
    break;
  }
  return false;
}

void game_inputs(SDL_Event event) {
  switch (event.type) {
  /* Closing the Window or pressing Escape will exit the program */
  case SDL_QUIT:
    break;
  case SDL_MOUSEMOTION:
    mouse.x = event.motion.x;
    mouse.y = event.motion.y;
    switch (event.button.button) {
    case SDL_BUTTON_LEFT:
      break;
    case SDL_BUTTON_RIGHT:
      break;
    case SDL_BUTTON_X1:
      break;
    default:
      break;
    }
    break;
  case SDL_MOUSEBUTTONDOWN:
    switch (event.button.button) {
    case SDL_BUTTON_LEFT:
      break;
    case SDL_BUTTON_RIGHT:
      break;
    default:
      break;
    }
    break;

  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      break;
    case SDLK_F1:
      break;
    case SDLK_LCTRL:
      lctrl = 1;
      break;
    case SDLK_LSHIFT:
      lshift = 1;
      break;
    case SDLK_LEFT:
      keydowns.left = true;
      break;
    case SDLK_RIGHT:
      keydowns.right = true;
      break;
    case SDLK_UP:
      keydowns.up = true;
      break;
    case SDLK_DOWN:
      keydowns.down = true;
      break;
    case SDLK_z:
      keydowns.secondary = true;
      break;
    case SDLK_x:
      keydowns.primary = true;
      break;
    case SDLK_RETURN:
      keydowns.pause = true;
      break;
    case SDLK_BACKSPACE:
      break;

    default:
      break;
    }
    break;

  case SDL_KEYUP:
    switch (event.key.keysym.sym) {
    case SDLK_LCTRL:
      lctrl = 0;
      break;
    case SDLK_LSHIFT:
      lshift = 0;
      break;
    case SDLK_LEFT:
      keydowns.left = false;
      break;
    case SDLK_RIGHT:
      keydowns.right = false;
      break;
    case SDLK_UP:
      keydowns.up = false;
      break;
    case SDLK_DOWN:
      keydowns.down = false;
      break;
    case SDLK_z:
      keydowns.secondary = false;
      break;
    case SDLK_x:
      keydowns.primary = false;
      break;
    case SDLK_RETURN:
      keydowns.pause = false;
      break;
    default:
      break;
    }
  case SDL_USEREVENT:
    break;

  default:
    break;
  }
}
