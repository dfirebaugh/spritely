#include "main.h"

void main_loop()
{
  process_inputs();
  render();
}

void color_picker_init_colors(context *ctx)
{
  char i;

  for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    ctx->pixels[i].color = i;
}

int main(int argc, char *args[])
{
  if (!init_everything())
    return -1;

  main_color = BLUE;
  sprite_canvas_ctx = context_make(SPRITE_CANVAS_PIXEL_SIZE, SPRITE_CANVAS_ROW_SIZE, SPRITE_CANVAS_ROW_SIZE, 0, 0);
  color_picker_ctx = context_make(COLORPICKER_PIXEL_SIZE, COLORPICKER_ROW_SIZE, COLORPICKER_ROW_SIZE, COLORPICKER_XOFFSET, 0);

  color_picker_init_colors(&color_picker_ctx);

  while (1)
    main_loop();
}
