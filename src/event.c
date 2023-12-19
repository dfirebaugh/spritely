
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "apprt.h"
#include "input.h"
#include "state.h"

bool do_event_polling(app_runtime a) {
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
      printf("keydown %d\n", event.key.keysym.sym == SDLK_o);
      break;
    case SDL_KEYUP:
      break;
    case SDL_USEREVENT:
      break;
    default:
      break;
    }
  }
  return false;
}
