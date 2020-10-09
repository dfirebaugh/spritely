#include "globals.h"

static int init_SDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return -1;
    }

    return 1;
}

static int create_window()
{
    window = NULL;


    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas"); /* disable key inputs in wasm */

    window = SDL_CreateWindow(
        "spritely",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return -1;
    }

    return 1;
}

static int create_renderer()
{
    renderer = NULL;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        return -1;
    }


    return 1;
}

static void setup_renderer()
{
    // Set size of renderer to the same as window
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Set color of renderer to green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
}

int init_everything()
{
  if (!init_SDL())
    return 0;

  if (!create_window())
    return 0;

  if (!create_renderer())
    return 0;

  setup_renderer();

  return 1;
}
