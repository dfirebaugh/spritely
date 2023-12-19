
#ifndef SPRITELY_INPUT_H
#define SPRITELY_INPUT_H

#include "apprt.h"

typedef struct Input *input;

extern bool input_is_mouse_down(input i);
extern void input_on_mouse_motion(app_runtime a, int x, int y);
extern void input_on_mouse_button_down(app_runtime a, unsigned char button);
extern void input_on_mouse_button_up(app_runtime a, unsigned char button);
extern void input_on_key_down(app_runtime a, int key_code);
extern void input_on_key_up(app_runtime a, int key_code);

#endif // SPRITELY_INPUT_H