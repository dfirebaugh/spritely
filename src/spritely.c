#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "app_state.h"
#include "defs.h"
#include "globals.h"
#include "js_api.h"
#include "sprite_editor.h"

void process_inputs(App_State_t spritely_state) {
  SDL_Event event;

  /* Loop through waiting messages and process them */
  while (SDL_PollEvent(&event)) {
    switch (App_State_get_state(spritely_state)) {
    case SPRITE_EDITOR:
      sprite_editor_inputs(event);
      break;
    case GAME:
      game_inputs(event);
      break;
    default:
      switch (event.type) {
      /* Closing the Window or pressing Escape will exit the program */
      case SDL_QUIT:
        exit(0);
        break;
      }
      break;
    }
  }
}

static void render(App_State_t spritely_state) {
  static Uint32 time_to_begin_render = 0;
  static Uint32 ticks;

  /* Limit rendering to 60Hz to save CPU and laptop battery life. */
  ticks = SDL_GetTicks();
  if (ticks < time_to_begin_render) {
    Uint32 delay_time = time_to_begin_render - ticks;
    if (delay_time > 16) /* in case of wrap-around, max delay is 16ms */
      delay_time = 16;
    SDL_Delay(delay_time);
  }

  switch (App_State_get_state(spritely_state)) {
  case SPRITE_EDITOR:
    sprite_editor_render();
    break;
  case GAME:
    if (js_draw != NULL)
      (*js_draw)();
    break;
  default:
    break;
  }

  SDL_RenderPresent(renderer);
  ticks = SDL_GetTicks();
  time_to_begin_render = ticks + 16; /* 16.6666ms is 60Hz */
}

void main_loop(void) {
  static unsigned int spritely_editor_initialized = 0;

  process_inputs(spritely_state);
  switch (App_State_get_state(spritely_state)) {
  case SHELL:
    break;
  case SPRITE_EDITOR:
    if (!spritely_editor_initialized)
      spritely_editor_initialized = sprite_editor_init();
    break;
  case GAME:
    if (js_update != NULL)
      (*js_update)();
    break;
  default:
    break;
  }

  render(spritely_state);
}

void emscripten_loop(void *arg) { main_loop(); }

void spritely_run(void) {
  spritely_state = App_State_make();

  App_State_set_state(spritely_state, SPRITE_EDITOR);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(emscripten_loop, NULL, -1, 1);
#else
  while (1) {
    main_loop();
  }
#endif
  App_State_free(spritely_state);
}
