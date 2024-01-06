#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"

struct graphics {
  SDL_Renderer *renderer;
  SDL_Window *window;
  int screen_width;
  int screen_height;
};

graphics graphics_init(int screen_width, int screen_height) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Could not initialize SDL2: %s\n", SDL_GetError());
    return NULL;
  }

  SDL_Window *window = SDL_CreateWindow(
      "spritely", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width,
      screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
    return NULL;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    fprintf(stderr, "Could not create a renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    return NULL;
  }

  if (SDL_RenderSetLogicalSize(renderer, screen_width, screen_height) != 0) {
    fprintf(stderr, "SDL_RenderSetLogicalSize failed: %s\n", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return NULL;
  }

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

  graphics g = malloc(sizeof(*g));
  if (g == NULL) {
    fprintf(stderr, "Failed to allocate memory for Graphics\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return NULL;
  }

  g->renderer = renderer;
  g->window = window;
  g->screen_width = screen_width;
  g->screen_height = screen_height;

  return g;
}

void graphics_destroy(graphics g) {
  SDL_DestroyRenderer(g->renderer);
  SDL_DestroyWindow(g->window);
  SDL_Quit();
  free(g);
}

void graphics_set_draw_color(graphics g, Uint8 r, Uint8 g1, Uint8 b, Uint8 a) {
  SDL_SetRenderDrawColor(g->renderer, r, g1, b, a);
}

SDL_Rect rect_to_sdl_rect(const Rect *rect) {
  SDL_Rect sdlRect;
  sdlRect.x = rect->x;
  sdlRect.y = rect->y;
  sdlRect.w = rect->w;
  sdlRect.h = rect->h;
  return sdlRect;
}

void graphics_draw_rect(graphics g, const Rect *rect) {
  SDL_Rect r = rect_to_sdl_rect(rect);
  SDL_RenderDrawRect(g->renderer, &r);
}

void graphics_fill_rect(graphics g, const Rect *rect) {
  SDL_Rect r = rect_to_sdl_rect(rect);
  SDL_RenderFillRect(g->renderer, &r);
}

void graphics_clear(graphics g) { SDL_RenderClear(g->renderer); }

void graphics_render_present(graphics g) { SDL_RenderPresent(g->renderer); }

int graphics_get_screen_width(graphics g) { return g->screen_width; }

int graphics_get_screen_height(graphics g) { return g->screen_height; }
