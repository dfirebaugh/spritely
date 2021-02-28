#include "main.h"


int main(int argc, char *args[])
{
  if (!init_everything())
    return -1;

  spritely_run();
}
