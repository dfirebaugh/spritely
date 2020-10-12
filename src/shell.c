
#include "globals.h"

#define IO_BUFFER_SIZE 50
#define CURSOR_WIDTH 22
#define LINE_HEIGHT 25
#define SHELL_ROWS 20

struct Shell
{
    unsigned int cursor;
    unsigned int line;
    int input[IO_BUFFER_SIZE];
    char output[IO_BUFFER_SIZE][SHELL_ROWS];
    char current_output[IO_BUFFER_SIZE];
};

const char alphanumeric[39] = "abcdefghijklmnopqrstuvwxyz 0123456789>\0";
typedef enum letters
{
    LETTER_A = 0,
    LETTER_B,
    LETTER_C,
    LETTER_D,
    LETTER_E,
    LETTER_F,
    LETTER_G,
    LETTER_H,
    LETTER_I,
    LETTER_J,
    LETTER_K,
    LETTER_L,
    LETTER_M,
    LETTER_N,
    LETTER_O,
    LETTER_P,
    LETTER_Q,
    LETTER_R,
    LETTER_S,
    LETTER_T,
    LETTER_U,
    LETTER_V,
    LETTER_W,
    LETTER_X,
    LETTER_Y,
    LETTER_Z,
    LETTER_SPACE,
    NUM_0,
    NUM_1,
    NUM_2,
    NUM_3,
    NUM_4,
    NUM_5,
    NUM_6,
    NUM_7,
    NUM_8,
    NUM_9
} letters_t;

const char *help_message = "type edit and press enter";
const char greater_than = '>';


void Shell_println(Shell_t shell, char *str);
static int find_index_of_alpha(char character);

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

    Sprite_sheet_render_sprite(alpha_chars_font, find_index_of_alpha(greater_than), 0 * CURSOR_WIDTH, LINE_HEIGHT * shell->line);
    // shell->cursor++;
    for (i = 0; i < IO_BUFFER_SIZE; ++i)
    {
        if (shell->input[i] > -1)
            Sprite_sheet_render_sprite(alpha_chars_font, shell->input[i], (i+1) * CURSOR_WIDTH, LINE_HEIGHT * shell->line);
    }

    for(int l = 0; l < strlen(shell->current_output); ++l)
    {
        if(find_index_of_alpha(shell->current_output[l]) < 0) continue;
        Sprite_sheet_render_sprite(alpha_chars_font, find_index_of_alpha(shell->current_output[l]), l * CURSOR_WIDTH, LINE_HEIGHT * 2);
    }
}

static void input_to_string(Shell_t shell, char *outStr)
{
    unsigned int i = 0;
    while (shell->input[i] != -1)
    {
        outStr[i] = alphanumeric[shell->input[i]];
        ++i;
    }
    outStr[i] = 0;
}

static void proccess_input(Shell_t shell)
{
    char input_command[IO_BUFFER_SIZE];
    input_to_string(shell, input_command);

    if (strcmp(input_command, "edit") == 0)
    {
        printf("editing\n");
        App_State_set_state(spritely_state, SPRITE_EDITOR);
    }

    if (strcmp(input_command, "help") == 0)
    {
        Shell_println(shell, (char *)help_message);
    } else {
        Shell_println(shell, input_command);
    }

    // shell->line++;
    shell->cursor = 0;
    for (int i = 0; i < IO_BUFFER_SIZE; ++i)
    {
        shell->input[i] = -1;
    }
}

void Shell_println(Shell_t shell, char *str)
{
    if (strlen(str) > IO_BUFFER_SIZE) return;
    memcpy(shell->current_output, str, IO_BUFFER_SIZE);
}

static int find_index_of_alpha(char character)
{
    const char *ptr = strchr(alphanumeric, character);
    if (ptr)
    {
        int index = ptr - alphanumeric;
        return index;
    }

    return -1;
}

static void register_keypress(Shell_t shell, letters_t letter)
{
    shell->input[shell->cursor] = letter;
    ++shell->cursor;
}

static void backspace(Shell_t shell)
{
    if(shell->cursor == 0) return;

    --shell->cursor;
    shell->input[shell->cursor] = -1;
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
        case SDLK_a:
            register_keypress(shell, LETTER_A);
            break;
        case SDLK_b:
            register_keypress(shell, LETTER_B);
            break;
        case SDLK_c:
            register_keypress(shell, LETTER_C);
            break;
        case SDLK_d:
            register_keypress(shell, LETTER_D);
            break;
        case SDLK_e:
            register_keypress(shell, LETTER_E);
            break;
        case SDLK_f:
            register_keypress(shell, LETTER_F);
            break;
        case SDLK_g:
            register_keypress(shell, LETTER_G);
            break;
        case SDLK_h:
            register_keypress(shell, LETTER_H);
            break;
        case SDLK_i:
            register_keypress(shell, LETTER_I);
            break;
        case SDLK_j:
            register_keypress(shell, LETTER_J);
            break;
        case SDLK_k:
            register_keypress(shell, LETTER_K);
            break;
        case SDLK_l:
            register_keypress(shell, LETTER_L);
            break;
        case SDLK_m:
            register_keypress(shell, LETTER_M);
            break;
        case SDLK_n:
            register_keypress(shell, LETTER_N);
            break;
        case SDLK_o:
            register_keypress(shell, LETTER_O);
            break;
        case SDLK_p:
            register_keypress(shell, LETTER_P);
            break;
        case SDLK_q:
            register_keypress(shell, LETTER_Q);
            break;
        case SDLK_r:
            register_keypress(shell, LETTER_R);
            break;
        case SDLK_s:
            register_keypress(shell, LETTER_S);
            break;
        case SDLK_t:
            register_keypress(shell, LETTER_T);
            break;
        case SDLK_u:
            register_keypress(shell, LETTER_U);
            break;
        case SDLK_v:
            register_keypress(shell, LETTER_V);
            break;
        case SDLK_w:
            register_keypress(shell, LETTER_W);
            break;
        case SDLK_x:
            register_keypress(shell, LETTER_X);
            break;
        case SDLK_y:
            register_keypress(shell, LETTER_Y);
            break;
        case SDLK_z:
            register_keypress(shell, LETTER_Z);
            break;
        case SDLK_0:
            register_keypress(shell, NUM_0);
            break;
        case SDLK_1:
            register_keypress(shell, NUM_1);
            break;
        case SDLK_2:
            register_keypress(shell, NUM_2);
            break;
        case SDLK_3:
            register_keypress(shell, NUM_3);
            break;
        case SDLK_4:
            register_keypress(shell, NUM_4);
            break;
        case SDLK_5:
            register_keypress(shell, NUM_5);
            break;
        case SDLK_6:
            register_keypress(shell, NUM_6);
            break;
        case SDLK_7:
            register_keypress(shell, NUM_7);
            break;
        case SDLK_8:
            register_keypress(shell, NUM_8);
            break;
        case SDLK_9:
            register_keypress(shell, NUM_9);
            break;
        case SDLK_RETURN:
            proccess_input(shell);
            break;
        case SDLK_SPACE:
            register_keypress(shell, LETTER_SPACE);
            break;
        case SDLK_LEFT:
            break;
        case SDLK_RIGHT:
            break;
        case SDLK_DOWN:
            break;
        case SDLK_UP:
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
