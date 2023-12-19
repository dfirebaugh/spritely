#include "state.h"

static app_state current_state;

void state_init(void) { current_state = RUNNING; }

app_state state_get(void) { return current_state; }

void state_set(enum app_state s) { current_state = s; }
