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

  init_sprite_canvas();
  init_color_picker();
  // init_select_window();

  while (1)
    main_loop();
}
