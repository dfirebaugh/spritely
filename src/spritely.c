#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "globals.h"

void process_inputs()
{
  SDL_Event event;

  /* Loop through waiting messages and process them */
  while (SDL_PollEvent(&event))
  {
    switch (App_State_get_state(spritely_state))
    {
    case SHELL:
      Shell_inputs(spritely_shell, event);
      break;
    case SPRITE_EDITOR:
      sprite_editor_inputs(event);
      break;
    case GAME:
      game_inputs(event);
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

static void render()
{
  switch (App_State_get_state(spritely_state))
  {
  case SHELL:
    Shell_render(spritely_shell);
    break;
  case SPRITE_EDITOR:
    sprite_editor_render();
    break;
  case GAME:
    if(js_draw != NULL)
      (*js_draw)();
    break;
  default:
    break;
  }

  SDL_RenderPresent(renderer);
}

void main_loop()
{
  process_inputs();
  switch (App_State_get_state(spritely_state))
  {
  case SHELL:
    break;
  case SPRITE_EDITOR:
    if (!spritely_editor_initialized)
      sprite_editor_init();
    break;
  case GAME:
    if(js_update != NULL)
      (*js_update)();
    break;
  default:
    break;
  }

  render();
}

void emscripten_loop(void *arg)
{
  main_loop();
}

void spritely_run()
{
  spritely_state = App_State_make();
  spritely_shell = Shell_make();
  icon_sprite_sheet = Sprite_sheet_make("assets/icons/icons.png");
  main_font_sprite_sheet = Sprite_sheet_make("assets/font/white_letter.sorted.png");

  spritely_entities = Entity_manager_make();

#ifdef __EMSCRIPTEN__
  App_State_set_state(spritely_state, SPRITE_EDITOR);
  emscripten_set_main_loop_arg(emscripten_loop, NULL, -1, 1);
#else
  while (1)
  {
    main_loop();
  }
#endif
}
