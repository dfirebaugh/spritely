#include "main.h"

void main_loop()
{
  process_inputs();
  render();
}

int main(int argc, char *args[])
{
  if (!init_everything())
    return -1;

  // Initlaize our player
  playerPos.x = 20;
  playerPos.y = 20;
  playerPos.w = 20;
  playerPos.h = 20;

  while (1)
    main_loop();
}
