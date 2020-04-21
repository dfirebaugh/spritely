#include "render.h"

void render()
{
    SDL_SetRenderDrawColor(renderer, 74, 50, 110, 255);
    SDL_RenderClear(renderer);

    context_render(&sprite_canvas_ctx);
    context_render(&color_picker_ctx);

    // render_color_picker();
    // render_select_window();

    SDL_RenderPresent(renderer);
}
