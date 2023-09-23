#include "defs.h"
#include "globals.h"

static int init_SDL(void) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    return -1;
  }

  return 1;
}

static int create_window(void) {
  window = NULL;

  window = SDL_CreateWindow(
      "spritely", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return -1;
  }

  return 1;
}

static int create_renderer(void) {
  renderer = NULL;

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    SDL_Log("Could not create a renderer: %s", SDL_GetError());
    return -1;
  }

  return 1;
}

static void setup_renderer(void) {
  // Set size of renderer to the same as window
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  // Set color of renderer to green
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
}

int init_everything(void) {
  if (!init_SDL())
    return 0;

  if (!create_window())
    return 0;

  if (!create_renderer())
    return 0;

  setup_renderer();

  return 1;
}
