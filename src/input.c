#include "globals.h"

static void free_all_contexts()
{

    Context_free(sprite_canvas_ctx);
    Context_free(sprite_selector_ctx);
    Context_free(color_picker_ctx);

    unsigned int i, j;
    unsigned int index = 0;
    for (i = 0; i < SPRITESHEET_COL_SIZE; i++)
    {
        for (j = 0; j < SPRITESHEET_ROW_SIZE; j++)
        {
            Context_free(sprite_selector_cells[index]);
            index++;
        }
    }
}

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
            free_all_contexts();
            Message_Queue_free(command_message_queue);
            Message_Queue_free(help_message_queue);
            exit(0);
            break;
        case SDL_MOUSEMOTION:
            mouse.x = event.motion.x;
            mouse.y = event.motion.y;
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                Draw_tool_handle_event(LEFT_CLICK_EVENT);
                break;
            case SDL_BUTTON_RIGHT:
                Draw_tool_handle_event(RIGHT_CLICK_EVENT);
                break;
            case SDL_BUTTON_X1:
                /* for some reason right mouse clicks are registering as SDL_BUTTON_X1 but only when I am moving the mouse.... ??????*/
                Draw_tool_handle_event(RIGHT_CLICK_EVENT);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                Draw_tool_handle_event(LEFT_CLICK_EVENT);
                break;
            case SDL_BUTTON_RIGHT:
                Draw_tool_handle_event(RIGHT_CLICK_EVENT);
                break;
            default:
                break;
            }
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                Message_Queue_free(command_message_queue);
                Message_Queue_free(help_message_queue);
                free_all_contexts();
                exit(0);
                break;
            case SDLK_s:
                if (lctrl) {
                    Draw_tool_handle_event(OPEN_FILE);
                }
                break;
            case SDLK_o:
                if (lctrl) {
                    Draw_tool_handle_event(OPEN_FILE);
                }
                break;
            case SDLK_F1:
                Draw_tool_handle_event(SHOW_HELP);
                break;
            case SDLK_LCTRL:
                lctrl = 1;
                break;
            case SDLK_LSHIFT:
                lshift = 1;
                break;
            case SDLK_c:
                if (lctrl)
                    Draw_tool_handle_event(COPY_SPRITE);
                break;
            case SDLK_v:
                if (lctrl)
                    Draw_tool_handle_event(PASTE_SPRITE);
                break;
            case SDLK_y:
                if (lctrl)
                    Draw_tool_handle_event(HANDLE_REDO);
                break;
            case SDLK_z:
                if (lctrl && !lshift)
                    Draw_tool_handle_event(HANDLE_UNDO);
                else if (lctrl && lshift)
                    Draw_tool_handle_event(HANDLE_REDO);
            case SDLK_f:
                Draw_tool_handle_event(ACTIVATE_FILL);
                break;
            case SDLK_SPACE:
                Draw_tool_handle_event(ACTIVATE_PEN);
                break;
            case SDLK_LEFT:
                Draw_tool_handle_event(LEFT_ARROW);
                break;
            case SDLK_RIGHT:
                Draw_tool_handle_event(RIGHT_ARROW);
                break;
            case SDLK_DOWN:
                Draw_tool_handle_event(DOWN_ARROW);
                break;
            case SDLK_UP:
                Draw_tool_handle_event(UP_ARROW);
                break;

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_LCTRL:
                lctrl = 0;
                break;
            case SDLK_LSHIFT:
                lshift = 0;
                break;

            default:
                break;
            }

        case SDL_USEREVENT:
            break;

        default:
            break;
        }
    }
}
