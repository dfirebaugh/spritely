
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "SDL_events.h"
#include "SDL_keycode.h"
#include "apprt.h"
#include "input.h"
#include "sprite_sheet.h"
#include "state.h"

typedef struct mouse {
  int x;
  int y;
  int canvas_x;
  int canvas_y;
} mouse_t;

struct Input {
  mouse_t mouse;
  bool mousedown;
  bool ctrl;
  bool shift;
};

bool mousedown;
mouse_t mouse;

bool input_is_mouse_down(void) { return mousedown; }

void input_on_mouse_button_down(app_runtime a, unsigned char button) {
  mousedown = true;

  switch (button) {
  case SDL_BUTTON_LEFT: {
    on_mouse_down_left(a, mouse.x, mouse.y);
    return;
  }
  case SDL_BUTTON_RIGHT:
    on_mouse_down_right(a, mouse.x, mouse.y);
    return;
  case SDL_BUTTON_X1:
    return;
  default:
    return;
  }
  return;
}

void input_on_mouse_button_up(app_runtime a, unsigned char button) {
  mousedown = false;

  switch (button) {
  case SDL_BUTTON_LEFT:
    on_mouse_up_left(a, mouse.x, mouse.y);
    return;
  case SDL_BUTTON_RIGHT:
    on_mouse_up_right(a, mouse.x, mouse.y);
    return;
  case SDL_BUTTON_X1:
    return;
  default:
    return;
  }
  if (mousedown)
    mousedown = false;
}

void input_on_mouse_motion(app_runtime a, int x, int y) {
  mouse.x = x;
  mouse.y = y;
  on_mouse_move(a, mouse.x, mouse.y);
}

bool is_left_control_down = false;

void input_on_key_down(app_runtime a, int key_code) {
  switch (key_code) {
  case SDLK_ESCAPE:
    state_set(SHOULD_EXIT);
    return;
  case SDLK_s:
    return;
  case SDLK_o:
    return;
  case SDLK_F1:
    return;
  case SDLK_LCTRL:
    is_left_control_down = true;
    return;
  case SDLK_LSHIFT:
    return;
  case SDLK_c:
    if (is_left_control_down) {
      copy_sprite_to_buffer(a);
    }
    return;
  case SDLK_v:
    if (is_left_control_down) {
      paste_sprite_from_buffer(a);
    }
    return;
  case SDLK_y:
    return;
  case SDLK_z:
  case SDLK_f:
    return;
  case SDLK_SPACE:
    return;
  case SDLK_LEFT:
    return;
  case SDLK_RIGHT:
    return;
  case SDLK_DOWN:
    return;
  case SDLK_UP:
    return;

  default:
    return;
  }
  return;
}

void input_on_key_up(app_runtime a, int key_code) {
  switch (key_code) {
  case SDLK_LCTRL:
    break;
  case SDLK_LSHIFT:
    break;

  default:
    break;
  }
}

bool do_polling(app_runtime a) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      state_set(SHOULD_EXIT);
      return true;
    case SDL_MOUSEMOTION:
      input_on_mouse_motion(a, event.motion.x, event.motion.y);
      break;
    case SDL_MOUSEBUTTONDOWN:
      input_on_mouse_button_down(a, event.button.button);
      break;
    case SDL_MOUSEBUTTONUP:
      input_on_mouse_button_up(a, event.button.button);
      break;
    case SDL_KEYDOWN:
      input_on_key_down(a, event.key.keysym.sym);
      // printf("keydown %d\n", event.key.keysym.sym == SDLK_o);
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_LCTRL:
        is_left_control_down = false;
        break;
      }
      break;
    case SDL_USEREVENT:
      break;
    default:
      break;
    }
  }
  return false;
}
