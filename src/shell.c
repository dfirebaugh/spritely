
#ifndef __EMSCRIPTEN__

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#endif
#include "globals.h"

#define IO_BUFFER_SIZE 50
#define CURSOR_WIDTH 22
#define LINE_HEIGHT 22
#define SHELL_ROWS 20

struct Shell
{
    unsigned int cursor;
    unsigned int line;
    char input[IO_BUFFER_SIZE];
    char output[SHELL_ROWS][IO_BUFFER_SIZE];
    char current_output[IO_BUFFER_SIZE];
#ifndef __EMSCRIPTEN__
    PyObject *pModule;
#endif
};

char *help_message = "type edit and press enter\0";

void Shell_println(Shell_t shell, char *str);
static void Shell_clear(Shell_t shell);

Shell_t Shell_make()
{
    Shell_t new_shell = malloc(sizeof(struct Shell));
    new_shell->cursor = 0;
    new_shell->line = 0;

    Shell_clear(new_shell);
    Shell_println(new_shell, help_message);

#ifndef __EMSCRIPTEN__
    Py_Initialize();
    new_shell->pModule = PyImport_AddModule("__main__"); //create main module
#endif

    return new_shell;
}

void Shell_free(Shell_t shell)
{
#ifndef __EMSCRIPTEN__
    Py_Finalize();
#endif
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
            if(shell->output[j][i] <= 0) continue;
            Sprite_sheet_render_sprite_scale(main_font_sprite_sheet, shell->output[j][i], i * CURSOR_WIDTH, LINE_HEIGHT * j, 2);
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

static void Shell_clear(Shell_t shell)
{
    for (int i = 0; i < IO_BUFFER_SIZE; ++i)
    {
        shell->input[i] = 0;
    }

    shell->line = 0;
    /* clear the output */
    for(uint8_t i = 0; i < SHELL_ROWS; ++i)
        for(uint8_t j = 0; j < IO_BUFFER_SIZE; ++j)
            shell->output[i][j] = 0;
}

static void execute_python_command(Shell_t shell, char *command)
{
#ifndef __EMSCRIPTEN__
    char *stdOutErr = "import sys\n\
class CatchOutErr:\n\
    def __init__(self):\n\
        self.value = ''\n\
    def write(self, txt):\n\
        self.value += txt\n\
catchOutErr = CatchOutErr()\n\
sys.stdout = catchOutErr\n\
sys.stderr = catchOutErr\n\
"; //this is python code to redirect stdouts/stderr

    PyRun_SimpleString(stdOutErr); //invoke code to redirect
    PyRun_SimpleString(command); //this is ok stdout
    // PyRun_SimpleString("1+a");    //this creates an error
    PyObject *catcher = PyObject_GetAttrString(shell->pModule,"catchOutErr"); //get our catchOutErr created above
    PyErr_Print(); //make python print any errors

    PyObject *output = PyObject_GetAttrString(catcher,"value"); //get the stdout and stderr from our catchOutErr
    PyObject* encoded = PyUnicode_AsEncodedString(output,"utf-8","strict");

    Shell_println(shell, PyBytes_AsString(encoded));
#endif
}

static void proccess_input(Shell_t shell)
{
    if (strcmp(shell->input, "help") == 0) {
        Shell_println(shell, help_message);
    } else if (strcmp(shell->input, "clear") == 0 || strcmp(shell->input, "cls") == 0) {
        Shell_clear(shell);
    } else if (strcmp(shell->input, "edit") == 0) {
        printf("editing\n");
        App_State_set_state(spritely_state, SPRITE_EDITOR);
    } else {
        execute_python_command(shell, shell->input);
    }

    shell->cursor = 0;
    /* clear the input */
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
    memcpy(shell->output[shell->line], str, strlen(str));
    ++shell->line;
}

static void register_keypress(Shell_t shell, char *text)
{
    SDL_strlcat(shell->input, text, sizeof(shell->input));
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
    case SDL_TEXTINPUT:
        if (SDL_strlen(event.text.text) == 0 || event.text.text[0] == '\n')
            break;

        register_keypress(shell, event.text.text);
        break;

    case SDL_USEREVENT:
        break;

    default:
        break;
    }
}
