#include "spritely.h"

char spritely_initialized = 0;

static void color_picker_init(context *ctx)
{
  char i;

  for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    ctx->pixels[i] = i;
}

#define SPRITE_SELECTOR_CELL_X_PADDING 6
#define SPRITE_SELECTOR_CELL_Y_PADDING 2

static void sprite_selector_init(context *ctx)
{
  char i, j;
  char index = 0;
  for (i = 0; i < SPRITESHEET_COL_SIZE; i++)
  {
    for (j = 0; j < SPRITESHEET_ROW_SIZE; j++)
    {
      int x = 1 + (j * SPRITESHEET_PIXEL_SIZE * (36 / SPRITESHEET_PIXEL_SIZE) + j) + SPRITE_SELECTOR_CELL_X_PADDING * j;
      int y = SPRITESHEET_YOFFSET + ((SPRITESHEET_PIXEL_SIZE * (SPRITE_CANVAS_SIZE / 8)) * i) + SPRITE_SELECTOR_CELL_Y_PADDING * i;

      ctx[index] = context_make(SPRITESHEET_PIXEL_SIZE,
                                SPRITE_CANVAS_ROW_SIZE,
                                SPRITE_CANVAS_ROW_SIZE,
                                x, y);
      index++;
    }
  }
}

static void render()
{
  SDL_SetRenderDrawColor(renderer, 74, 50, 110, 255);
  SDL_RenderClear(renderer);

  context_render(&sprite_canvas_ctx);
  context_render(&color_picker_ctx);

  char i;
  for (i = 0; i < SPRITESHEET_SIZE; i++)
  {
    context_render(sprite_selector_cells + i);
  }

#ifdef __DEBUG_SPRITE_SELECTOR__
  context_render(&sprite_selector_ctx);
#endif
  context_render(&sprite_selection_indicator);

  SDL_RenderPresent(renderer);
}

extern void spritely_run()
{
  if (!spritely_initialized)
  {
    pen_color = BLUE;
    sprite_canvas_ctx = context_make(SPRITE_CANVAS_PIXEL_SIZE,
                                     SPRITE_CANVAS_ROW_SIZE, SPRITE_CANVAS_ROW_SIZE, 0, 0);
    sprite_selector_ctx = context_make(SPRITESHEET_CELL_SIZE,
                                       SPRITESHEET_ROW_SIZE, SPRITESHEET_COL_SIZE, 0, SPRITESHEET_YOFFSET);
    color_picker_ctx = context_make(COLORPICKER_PIXEL_SIZE,
                                    COLORPICKER_ROW_SIZE, COLORPICKER_ROW_SIZE,
                                    COLORPICKER_XOFFSET, COLORPICKER_YOFFSET);


    sprite_selector_cells[SPRITESHEET_SIZE];
    sprite_selector_init(sprite_selector_cells);
    color_picker_init(&color_picker_ctx);

    sprite_selection_indicator = context_make(0, 1, 1, 0, 0);
    sprite_selection_indicator.pixels[0] = 1;
    context_focus(&sprite_selection_indicator, sprite_selector_cells);
    spritely_initialized = 1;
  }

  while (1)
  {
    process_inputs();

    render();
  }
}
