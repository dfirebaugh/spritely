
#include "globals.h"

#define IO_BUFFER_SIZE 50
#define CURSOR_WIDTH 22
#define LINE_HEIGHT 25
#define SHELL_ROWS 20

struct Shell
{
    unsigned int cursor;
    unsigned int line;
    char input[IO_BUFFER_SIZE];
    char output[IO_BUFFER_SIZE][SHELL_ROWS];
    char current_output[IO_BUFFER_SIZE];
};

const char *help_message = "type edit and press enter";
const char greater_than = '>';

void Shell_println(Shell_t shell, char *str);

Shell_t Shell_make()
{
    Shell_t new_shell = malloc(sizeof(struct Shell));
    new_shell->cursor = 0;
    new_shell->line = 0;

    for (int i = 0; i < IO_BUFFER_SIZE; ++i)
    {
        new_shell->input[i] = -1;
    }

    memcpy(new_shell->current_output, help_message, IO_BUFFER_SIZE);

    return new_shell;
}

void Shell_free(Shell_t shell)
{
    free(shell);
}

void Shell_render(Shell_t shell)
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    unsigned int i;

    Sprite_sheet_render_sprite(main_font_sprite_sheet, greater_than, 0 * CURSOR_WIDTH, LINE_HEIGHT * shell->line);

    for (i = 0; i < IO_BUFFER_SIZE; ++i)
    {
        if (shell->input[i] > 0)
            Sprite_sheet_render_sprite(main_font_sprite_sheet, shell->input[i], (i+1) * CURSOR_WIDTH, LINE_HEIGHT * shell->line);
    }

    for(int l = 0; l < strlen(shell->current_output); ++l)
    {
        if(shell->current_output[l] == 0) continue;
        Sprite_sheet_render_sprite(main_font_sprite_sheet, shell->current_output[l], l * CURSOR_WIDTH, LINE_HEIGHT * 2);
    }
}

static void proccess_input(Shell_t shell)
{
    if (strcmp(shell->input, "edit") == 0)
    {
        printf("editing\n");
        App_State_set_state(spritely_state, SPRITE_EDITOR);
    }

    if (strcmp(shell->input, "help") == 0)
    {
        Shell_println(shell, (char *)help_message);
    } else {
        Shell_println(shell, shell->input);
    }

    // shell->line++;
    shell->cursor = 0;
    for (int i = 0; i < IO_BUFFER_SIZE; ++i)
    {
        shell->input[i] = 0;
    }
}

void Shell_println(Shell_t shell, char *str)
{
    if (strlen(str) > IO_BUFFER_SIZE) return;
    memcpy(shell->current_output, str, IO_BUFFER_SIZE);
}

static void register_keypress(Shell_t shell, char letter)
{
    shell->input[shell->cursor] = letter;
    shell->input[shell->cursor+1] = 0;
    ++shell->cursor;
}

static void backspace(Shell_t shell)
{
    if(shell->cursor == 0) return;

    --shell->cursor;
    shell->input[shell->cursor] = 0;
}

void Shell_inputs(Shell_t shell, SDL_Event event)
{
    switch (event.type)
    {
    /* Closing the Window or pressing Escape will exit the program */
    case SDL_QUIT:
        Shell_free(shell);
        free_globals();
        exit(0);
        break;
    case SDL_MOUSEMOTION:
        mouse.x = event.motion.x;
        mouse.y = event.motion.y;
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            break;
        case SDL_BUTTON_RIGHT:
            break;
        case SDL_BUTTON_X1:
            break;
        default:
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            break;
        case SDL_BUTTON_RIGHT:
            break;
        default:
            break;
        }
        break;

    case SDL_KEYDOWN:
        if(event.key.keysym.sym >= ' ' && event.key.keysym.sym <= 'z')
            register_keypress(shell, event.key.keysym.sym);

        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            Shell_free(shell);
            free_globals();
            exit(0);
            break;
        case SDLK_F1:
            break;
        case SDLK_LCTRL:
            lctrl = 1;
            break;
        case SDLK_LSHIFT:
            lshift = 1;
            break;
        case SDLK_RETURN:
            proccess_input(shell);
            break;
        case SDLK_BACKSPACE:
            backspace(shell);
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
