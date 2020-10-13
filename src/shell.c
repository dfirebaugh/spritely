
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
    char output[SHELL_ROWS][IO_BUFFER_SIZE];
    char current_output[IO_BUFFER_SIZE];
};

char *help_message = "type edit and press enter\0";

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

    Shell_println(new_shell, help_message);

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

    uint8_t i;
    uint8_t j;

    for(i = 0; i < IO_BUFFER_SIZE; ++i)
    {
        for(j = 0; j < SHELL_ROWS; ++j)
        {
            if(shell->output[j][i] < 0) continue;
            Sprite_sheet_render_sprite(main_font_sprite_sheet, shell->output[j][i], i * CURSOR_WIDTH, LINE_HEIGHT * j);
        }
    }
}

static void input_to_render(Shell_t shell)
{
    char result[IO_BUFFER_SIZE];

    uint32_t i;
    result[0] = '>';
    for(i = 1; i < IO_BUFFER_SIZE; ++i)
    {
        result[i] = shell->input[i-1];
    }

    memcpy(shell->output[shell->line], result, IO_BUFFER_SIZE);
}

static void proccess_input(Shell_t shell)
{
    if (strcmp(shell->input, "help") == 0) {
        Shell_println(shell, help_message);
    } else if (strcmp(shell->input, "clear") == 0) {
        shell->line = 0;

        /* clear the output */
        for(uint32_t i = 0; i < SHELL_ROWS; ++i)
            for(uint32_t j = 0; j < IO_BUFFER_SIZE; ++j)
                shell->output[i][j] = 0;
    } else if (strcmp(shell->input, "edit") == 0) {
        printf("editing\n");
        App_State_set_state(spritely_state, SPRITE_EDITOR);
    } else {
        Shell_println(shell, shell->input);
    }

    shell->cursor = 0;
    for (int i = 0; i < IO_BUFFER_SIZE; ++i)
    {
        shell->input[i] = 0;
    }
    memcpy(shell->output[shell->line], shell->input, IO_BUFFER_SIZE);
    input_to_render(shell);
}

void Shell_println(Shell_t shell, char *str)
{
    if (strlen(str) > IO_BUFFER_SIZE) return;
    ++shell->line;
    memcpy(shell->output[shell->line], str, IO_BUFFER_SIZE);
    printf("%s\n", shell->output[shell->line]);
}

static void register_keypress(Shell_t shell, char letter)
{
    shell->input[shell->cursor] = letter;
    shell->input[shell->cursor+1] = 0;
    ++shell->cursor;
    input_to_render(shell);
}

static void backspace(Shell_t shell)
{
    if(shell->cursor == 0) return;

    --shell->cursor;
    shell->input[shell->cursor] = 0;
    input_to_render(shell);
}

void Shell_inputs(Shell_t shell, SDL_Event event)
{
    switch (event.type)
    {
    /* Closing the Window or pressing Escape will exit the program */
    case SDL_QUIT:
        Shell_free(shell);
        // free_globals();
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
