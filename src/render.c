#include "render.h"

void render()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // Clear winow
    SDL_RenderClear(renderer);
    // Set render color to blue ( rect will be rendered in this color )
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &playerPos);
    SDL_RenderPresent(renderer);
}
