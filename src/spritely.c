#include "spritely.h"

void render()
{
  SDL_SetRenderDrawColor(renderer, 74, 50, 110, 255);
  SDL_RenderClear(renderer);

  context_render(&sprite_canvas_ctx);
  context_render(&color_picker_ctx);

    char i;
  for (i = 0; i < SPRITESHEET_SIZE; i++)
  {
    context_render(sprite_sheet_cells+i);
  }
  // context_render(&select_window_ctx);

  SDL_RenderPresent(renderer);
}

static void color_picker_init_colors(context *ctx)
{
  char i;

  for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    ctx->pixels[i].color = i;
}

static void sprite_cells_setup()
{
  char x_padding = 6;
  char y_padding = 2;
  char i, j;
  char index = 0;
  for (i = 0; i < SPRITESHEET_COL_SIZE; i++)
  {
      for (j = 0; j < SPRITESHEET_ROW_SIZE; j++)
      {
        int x = 1 + (j * SPRITESHEET_PIXEL_SIZE * (36/SPRITESHEET_PIXEL_SIZE) + j) + x_padding * j;
        int y = SPRITESHEET_YOFFSET + ((SPRITESHEET_PIXEL_SIZE * (SPRITE_CANVAS_SIZE/8)) * i) + y_padding * i;

        sprite_sheet_cells[index] = context_make(SPRITESHEET_PIXEL_SIZE, 
          SPRITE_CANVAS_ROW_SIZE, SPRITE_CANVAS_ROW_SIZE, x, y);

            sprite_sheet_cells[index].pixels[index].color = i % 12;

        printf("x: %d, y: %d\n", x, y);
        index++;
      }
  }
}

void spritely_init()
{
  main_color = BLUE;
  sprite_canvas_ctx = context_make(SPRITE_CANVAS_PIXEL_SIZE, SPRITE_CANVAS_ROW_SIZE, SPRITE_CANVAS_ROW_SIZE, 0, 0);
  select_window_ctx = context_make(SPRITESHEET_CELL_SIZE, SPRITESHEET_ROW_SIZE, SPRITESHEET_COL_SIZE, 0, SPRITESHEET_YOFFSET);
  color_picker_ctx = context_make(COLORPICKER_PIXEL_SIZE, COLORPICKER_ROW_SIZE, COLORPICKER_ROW_SIZE, COLORPICKER_XOFFSET, COLORPICKER_YOFFSET);
  color_picker_init_colors(&color_picker_ctx);

  sprite_cells_setup();
}

void spritely_run()
{
  process_inputs();
  render();
}
