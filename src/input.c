#include "input.h"

void process_inputs()
{
    SDL_Event event;

    /* Loop through waiting messages and process them */
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {

        /* Closing the Window or pressing Escape will exit the program */
        case SDL_QUIT:
            exit(0);
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                exit(0);
                break;

            case SDLK_s:
            case SDLK_DOWN:
                ++playerPos.y;
                break;

            case SDLK_d:
            case SDLK_RIGHT:
                ++playerPos.x;
                break;

            case SDLK_a:
            case SDLK_LEFT:
                --playerPos.x;
                break;

            case SDLK_w:
            case SDLK_UP:
                --playerPos.y;
                break;

            case SDLK_r:
                break;

            case SDLK_SPACE:
                break;

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            break;

        case SDL_USEREVENT:
            break;

        default:
            break;
        }
    }
}
