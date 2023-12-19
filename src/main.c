#include "apprt.h"
#include "state.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void spritely_run(void);

app_runtime a;

int main(int argc, char *args[]) {
  a = app_runtime_init();
  spritely_run();
  return 0;
}

void main_loop(void) { app_runtime_run(a); }

#ifdef __EMSCRIPTEN__
void emscripten_loop(void *arg) { main_loop(); }
#endif

void spritely_run(void) {
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(emscripten_loop, NULL, -1, 1);
#else
  while (!app_runtime_should_exit(a)) {
    main_loop();
  }
#endif
  app_runtime_destroy_and_exit(a);
}
