#include "spritely.h"

void color_picker_init_colors(context *ctx)
{
  char i;

  for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    ctx->pixels[i].color = i;
}


void spritely_init()
{
    main_color = BLUE;
    sprite_canvas_ctx = context_make(SPRITE_CANVAS_PIXEL_SIZE, SPRITE_CANVAS_ROW_SIZE, SPRITE_CANVAS_ROW_SIZE, 0, 0);
    color_picker_ctx = context_make(COLORPICKER_PIXEL_SIZE, COLORPICKER_ROW_SIZE, COLORPICKER_ROW_SIZE, COLORPICKER_XOFFSET, 0);

    color_picker_init_colors(&color_picker_ctx);
}

void spritely_run()
{
  process_inputs();
  render();
}
