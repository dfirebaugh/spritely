#ifndef _APPSTATE
#define _APPSTATE

typedef struct App_State *App_State_t;

typedef enum state_type { SHELL = 0, SPRITE_EDITOR, GAME } state_type_t;

extern App_State_t App_State_make(void);
extern void App_State_free(App_State_t state);
extern void App_State_set_state(App_State_t state, state_type_t type);
extern state_type_t App_State_get_state(App_State_t state);

#endif
