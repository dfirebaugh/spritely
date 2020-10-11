#ifndef _SHELL
#define _SHELL

typedef struct Shell *Shell_t;

extern Shell_t Shell_make();
extern void Shell_inputs(Shell_t shell, SDL_Event event);
extern void Shell_render(Shell_t shell);
extern void Shell_free(Shell_t shell);

#endif
