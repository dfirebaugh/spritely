#include <SDL2/SDL.h>
#include <stdint.h>

#include "graphics.h"

struct graphics {
  SDL_Renderer *renderer;
  SDL_Window *window;
  int screen_width;
  int screen_height;
};

static int init_SDL(void) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    return -1;
  }

  return 1;
}

graphics graphics_init(int screen_width, int screen_height) {
  if (!init_SDL())
    return NULL;
  SDL_Window *window = SDL_CreateWindow(
      "spritely", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width,
      screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return NULL;
  }
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    SDL_Log("Could not create a renderer: %s", SDL_GetError());
    return NULL;
  }

  SDL_RenderSetLogicalSize(renderer, screen_width, screen_height);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

  graphics g = malloc(sizeof(struct graphics));
  if (!g)
    return NULL;

  g->renderer = renderer;
  g->window = window;
  g->screen_width = screen_width;
  g->screen_height = screen_height;

  return g;
}

void graphics_destroy(graphics g) {
  free(g->renderer);
  free(g->window);
  free(g);
}

int graphics_get_screen_width(graphics g) { return g->screen_width; }
int graphics_get_screen_height(graphics g) { return g->screen_height; }

void graphics_set_draw_color(graphics gfx, uint8_t r, uint8_t g, uint8_t b,
                             uint8_t a) {
  SDL_SetRenderDrawColor(gfx->renderer, r, g, b, a);
}

static SDL_Rect rect_to_sdl_rect(const Rect *rect) {
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
