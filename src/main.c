#include "main.h"


int main(int argc, char *args[])
{
#ifndef __EMSCRIPTEN__
  if (argc > 1) {
    if(strcmp(get_filename_ext(args[1]), "spit") == 0)
      run_spit_file(args[1]);
  }
#endif

  if (!init_everything())
    return -1;

  spritely_run();
}
