#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "globals.h"
#include "app_state.h"
#include "sprite_editor.h"

App_State_t spritely_state;

void process_inputs(App_State_t spritely_state)
{
  SDL_Event event;

  /* Loop through waiting messages and process them */
  while (SDL_PollEvent(&event))
  {
    switch (App_State_get_state(spritely_state))
    {
    case SPRITE_EDITOR:
      sprite_editor_inputs(event);
      break;
    default:
      switch (event.type)
      {
      /* Closing the Window or pressing Escape will exit the program */
      case SDL_QUIT:
        exit(0);
        break;
      }
      break;
    }
  }
}

static void render(App_State_t spritely_state)
{
  switch (App_State_get_state(spritely_state))
  {
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
}

void main_loop()
{
  static unsigned int spritely_editor_initialized = 0;

  process_inputs(spritely_state);
  switch (App_State_get_state(spritely_state))
  {
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

void emscripten_loop(void *arg)
{
  main_loop();
}

void spritely_run()
{
  spritely_state = App_State_make();

  App_State_set_state(spritely_state, SPRITE_EDITOR);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(emscripten_loop, NULL, -1, 1);
#else
  while (1)
  {
    main_loop();
  }
#endif
  App_State_free(spritely_state);
}
