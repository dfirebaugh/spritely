#include "util.h"

int XYInRect(const SDL_Rect rect)
{
    return ((mouse.x >= rect.x && mouse.x <= rect.x + rect.w) && (mouse.y >= rect.y && mouse.y <= rect.y + rect.h));
}

void set_pixel_render_color(pixel p)
{
    switch (p.color)
    {
    case D_BLUE:
        SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
        break;
    case B_BLUE:
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        break;
    case B_RED:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        break;
    case BLUE:
        SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
        break;
    case GREEN:
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        break;
    case RED:
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        break;
    case BLACK:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        break;
    case GREY1:
        SDL_SetRenderDrawColor(renderer, 224, 224, 224, 255);
        break;
    case GREY2:
        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
        break;
    case GREY4:
        SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
        break;
    case GREY8:
        SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);
        break;
    case GREY16:
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        break;
    case WHITE:
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        break;
    case CYAN:
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        break;
    case YELLOW:
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        break;
    case MAGENTA:
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        break;
    default:
        break;
        ;
    }
}