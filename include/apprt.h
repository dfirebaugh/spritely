

#ifndef APP_RUNTIME_H
#define APP_RUNTIME_H

#include <stdbool.h>

typedef struct app_runtime *app_runtime;

extern app_runtime app_runtime_init(void);
extern void app_runtime_run(app_runtime a);
extern void app_runtime_destroy_and_exit(app_runtime a);
extern bool app_runtime_should_exit(app_runtime a);
extern void on_mouse_down_right(app_runtime a, int x, int y);
extern void on_mouse_down_left(app_runtime a, int x, int y);
extern void on_mouse_up_right(app_runtime a, int x, int y);
extern void on_mouse_up_left(app_runtime a, int x, int y);
extern void on_mouse_move(app_runtime a, int x, int y);

#endif // APP_RUNTIME_H
