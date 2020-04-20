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
        case SDL_MOUSEMOTION:
            mouse.x = event.motion.x;
            mouse.y = event.motion.y;
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                sprite_canvas_left_click();
                color_picker_click();
                // select_window_click();
                break;
            case SDL_BUTTON_RIGHT:
                sprite_canvas_right_click();
                color_picker_click();
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                sprite_canvas_left_click();
                color_picker_click();
                // select_window_click();
                break;
            case SDL_BUTTON_RIGHT:
                sprite_canvas_right_click();
                color_picker_click();
                break;
            default:
                break;
            }
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                exit(0);
                break;

            case SDLK_s:
            case SDLK_DOWN:
                break;

            case SDLK_d:
            case SDLK_RIGHT:
                break;

            case SDLK_a:
            case SDLK_LEFT:
                break;

            case SDLK_w:
            case SDLK_UP:
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
