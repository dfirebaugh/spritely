
#ifndef SPRITELY_STATE_H
#define SPRITELY_STATE_H

typedef enum app_state {
  SHOULD_EXIT = -1,
  RUNNING,
  SPRITE_EDITOR,
} app_state;


extern void state_init(void);
extern app_state state_get(void);
extern void state_set(enum app_state);

#endif // SPRITELY_STATE_H
