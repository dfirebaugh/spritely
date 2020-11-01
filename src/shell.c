
#include "globals.h"

#define IO_BUFFER_SIZE 50
#define CURSOR_WIDTH 22
#define LINE_HEIGHT 22
#define SHELL_ROWS 20
#define SHELL_HISTORY_NUM 20
#define SHELL_CHAR_WIDTH 28

/*
  Proper message when an invalid command is submitted
  Currently, invalid commands are handled as python commands 
*/
#define HANDLE_INVALID_CMD 0

typedef struct Command_History {
    const char *cmd; /**< Command string */
    struct Command_History *prev; /**< Previous command */
    struct Command_History *next; /**< Next command */
} *Command_History_t;

typedef struct Shell_Command_History
{
    unsigned int cnt; /**< Number of commands in the command history */
    Command_History_t head; /**< Pointer to the first command in the command history */
    Command_History_t tail; /**< Pointer to the last command in the command history */
    Command_History_t cur; /**< Command history cursor */
} *Shell_Command_History_t;

struct Shell
{
    unsigned int cursor;
    unsigned int line;
    char input[IO_BUFFER_SIZE];
    char output[SHELL_ROWS][IO_BUFFER_SIZE];
    char current_output[IO_BUFFER_SIZE];
    struct Shell_Command_History cmd_history;
#ifndef __EMSCRIPTEN__
    PyObject *pModule;
#endif
};

typedef void (*Command_Hander)(Shell_t);

typedef struct Shell_Command
{
    const char* name; /**< Command string */
    const Command_Hander handler; /**< Function pointer to the command handler */
} *Shell_Command_t;

char *help_message = "type edit and press enter\0";

void Shell_println(Shell_t shell, char *str);
static void Shell_clear(Shell_t shell);

static void Shell_tab_complete(Shell_t shell);
static void Shell_history_append(Shell_t shell, const char* cmd);
static void Shell_history_clear(Shell_t shell);
static void Shell_history_show_prev(Shell_t shell);
static void Shell_history_show_next(Shell_t shell);

/* Command handlers */
static void Shell_cmd_help(Shell_t shell);
static void Shell_cmd_edit(Shell_t shell);
static void Shell_cmd_clear(Shell_t shell);
static void Shell_cmd_exit(Shell_t shell);
#if HANDLE_INVALID_CMD
static void Shell_cmd_invalid(Shell_t shell);
#endif /*HANDLE_INVALID_CMD*/

/* All supported shell commands are defined here */
static const struct Shell_Command shell_commands[] = {
    {"help",  &Shell_cmd_help},
    {"clear", &Shell_cmd_clear},
    {"cls",   &Shell_cmd_clear},
    {"edit",  &Shell_cmd_edit},
    {"exit",  &Shell_cmd_exit},
    {NULL,NULL}
};

Shell_t Shell_make()
{
    Shell_t new_shell = malloc(sizeof(struct Shell));
    new_shell->cursor = 0;
    new_shell->line = 0;

    new_shell->cmd_history.cnt = 0;
    new_shell->cmd_history.head = NULL;
    new_shell->cmd_history.tail = NULL;
    new_shell->cmd_history.cur = NULL;

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
    Shell_history_clear(shell);
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
    int is_cmd_found = 0;
    Shell_Command_t cmd = (Shell_Command_t)shell_commands;
    while(NULL != cmd->name && NULL != cmd->handler)
    {
        if (strcmp(shell->input, cmd->name) == 0)
        {
            cmd->handler(shell);
            Shell_history_append(shell, cmd->name);
            is_cmd_found = 1;
            break;
        }
        ++cmd;
    }
    if(!is_cmd_found) {
#if HANDLE_INVALID_CMD
        Shell_cmd_invalid(shell);
#endif /*HANDLE_INVALID_CMD*/
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
    int idx = 0;
    const int len = strlen(str);

    if(str == NULL || len == 0) return;

    ++shell->line;

    for(int i = 0; i < len; ++i)
    {
        char c = str[i];

        /* line delimeter, line wrapping */
        if(c == '\n' || idx == SHELL_CHAR_WIDTH)
        {
            ++shell->line;
            idx = 0;
            if(c == '\n') continue;
        }
        else if(c == 0)
        {
            break;
        }
        shell->output[shell->line][idx++] = c;
    }
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
        case SDLK_TAB:
            Shell_tab_complete(shell);
            break;
        case SDLK_UP:
            Shell_history_show_prev(shell);
            break;
        case SDLK_DOWN:
            Shell_history_show_next(shell);
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

/**
 * @brief Tab completion
 */
static void Shell_tab_complete(Shell_t shell)
{
    Shell_Command_t cmd = (Shell_Command_t)shell_commands;
    while(NULL != cmd->name && NULL != cmd->handler)
    {
        const size_t num_chars = strlen(shell->input);
        if (memcmp(shell->input, cmd->name, num_chars) == 0)
        {
            strcpy(shell->input, cmd->name);
            shell->cursor = strlen(shell->input);
            input_to_render(shell);
            break;
        }
        ++cmd;
    }
}

static void Shell_cmd_help(Shell_t shell)
{
    Shell_Command_t cmd = (Shell_Command_t)shell_commands;
    Shell_println(shell, "Commands:");

    while(NULL != cmd->name && NULL != cmd->handler)
    {
        Shell_println(shell, (char*)cmd->name);
        ++cmd;
    }
}

static void Shell_cmd_edit(Shell_t shell)
{
    printf("editing\n");
    App_State_set_state(spritely_state, SPRITE_EDITOR);
}

static void Shell_cmd_clear(Shell_t shell)
{
    Shell_clear(shell);
}

#if HANDLE_INVALID_CMD
static void Shell_cmd_invalid(Shell_t shell)
{
    Shell_println(shell, "invalid command\ntype help to see all commands");
}
#endif /*HANDLE_INVALID_CMD*/

static void Shell_cmd_exit(Shell_t shell)
{
    Shell_free(shell);
    exit(0);
}

/**
 * @brief Append a command to the command history
 * @param cmd Command string to be appended
 */
static void Shell_history_append(Shell_t shell, const char* cmd)
{
    Command_History_t cmd_hist = (Command_History_t)malloc(sizeof(struct Command_History));
    cmd_hist->next = NULL;
    cmd_hist->prev = NULL;
    cmd_hist->cmd = cmd;
    shell->cmd_history.cur = NULL; /* reset the command history cursor */

    /* command history is empty */
    if(NULL == shell->cmd_history.head)
    {
        shell->cmd_history.head = cmd_hist;
        shell->cmd_history.tail = cmd_hist;
    }
    else
    {
        if(strcmp(shell->cmd_history.tail->cmd, cmd) == 0)
        {
            /* if same as the last command, no need to append into the history list */
            free(cmd_hist);
            return;
        }
        cmd_hist->prev = shell->cmd_history.tail;
        shell->cmd_history.tail->next = cmd_hist;
        shell->cmd_history.tail = shell->cmd_history.tail->next;
    }

    if(shell->cmd_history.cnt < SHELL_HISTORY_NUM)
    {
        ++shell->cmd_history.cnt;
    }
    else /* command history is full */
    {
        /* clean up the first command in history list */
        cmd_hist = shell->cmd_history.head->next;
        free(shell->cmd_history.head);
        cmd_hist->prev = NULL;
        shell->cmd_history.head = cmd_hist;
    }
}

static void Shell_history_clear(Shell_t shell)
{
    Command_History_t cmd_hist = NULL;

    while(shell->cmd_history.head != NULL)
    {
        cmd_hist = shell->cmd_history.head->next;
        free(shell->cmd_history.head);
        shell->cmd_history.head = cmd_hist;
    }
}

/**
 * @brief Show the previous command in the command history
 */
static void Shell_history_show_prev(Shell_t shell)
{
    if(NULL == shell->cmd_history.cur)
    {
        if(NULL != shell->cmd_history.tail)
        {
            shell->cmd_history.cur = shell->cmd_history.tail;
        }
        else
        {
            /* command history is empty */
            return;
        }
    }
    else if(shell->cmd_history.head == shell->cmd_history.cur)
    {
        /* reach first command */
        return;
    }
    else
    {
        shell->cmd_history.cur = shell->cmd_history.cur->prev;
    }

    memset(shell->input, 0, IO_BUFFER_SIZE);
    strcpy(shell->input, shell->cmd_history.cur->cmd);
    shell->cursor = strlen(shell->input);
    input_to_render(shell);
}

/**
 * @brief Show the next command in the command history
 */
static void Shell_history_show_next(Shell_t shell)
{
    if(NULL == shell->cmd_history.cur)
    {
        /* no more command to show */
        return;
    }
    else
    {
        shell->cmd_history.cur = shell->cmd_history.cur->next;
    }

    /* clear input buffer */
    memset(shell->input, 0, IO_BUFFER_SIZE);

    if(NULL == shell->cmd_history.cur)
    {
        /* reach the last command in the command history, show blank line */
        shell->cursor = 0;
    }
    else
    {
        strcpy(shell->input, shell->cmd_history.cur->cmd);
        shell->cursor = strlen(shell->input);
    }

    input_to_render(shell);
}
