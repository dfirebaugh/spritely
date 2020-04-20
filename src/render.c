#include "render.h"

void render()
{
    SDL_SetRenderDrawColor(renderer, 74, 50, 110, 255);
    SDL_RenderClear(renderer);

    render_sprite_ctx();
    render_color_picker();

    SDL_RenderPresent(renderer);
}
