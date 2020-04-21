#include "spritely.h"

static void color_picker_init(context *ctx);
static void sprite_selector_init(context *ctx);
static void render(context sprite_canvas_ctx, context color_picker_ctx,
                   context select_window_ctx, context *sprite_sheet_cells, context sprite_selection_cursor);

char spritely_initialized = 0;

extern void spritely_run()
{
  if (!spritely_initialized)
  {
    main_color = BLUE;
    sprite_canvas_ctx = context_make(SPRITE_CANVAS_PIXEL_SIZE,
                                     SPRITE_CANVAS_ROW_SIZE, SPRITE_CANVAS_ROW_SIZE, 0, 0);
    select_window_ctx = context_make(SPRITESHEET_CELL_SIZE,
                                     SPRITESHEET_ROW_SIZE, SPRITESHEET_COL_SIZE, 0, SPRITESHEET_YOFFSET);
    color_picker_ctx = context_make(COLORPICKER_PIXEL_SIZE,
                                    COLORPICKER_ROW_SIZE, COLORPICKER_ROW_SIZE,
                                    COLORPICKER_XOFFSET, COLORPICKER_YOFFSET);

    sprite_sheet_cells[SPRITESHEET_SIZE];
    sprite_selector_init(sprite_sheet_cells);
    color_picker_init(&color_picker_ctx);

    spritely_initialized = 1;
  sprite_selection_cursor = context_make(0, 1, 1,0, 0);
  sprite_selection_cursor.pixels[0] = 1;
  memcpy(sprite_selection_cursor.rects, sprite_sheet_cells[0].rects, sizeof(sprite_selection_cursor.rects));
  }


  while (1)
  {
    process_inputs();

    render(sprite_canvas_ctx, color_picker_ctx, select_window_ctx,
           sprite_sheet_cells, sprite_selection_cursor);
  }
}

static void render(context sprite_canvas_ctx, context color_picker_ctx,
                   context select_window_ctx, context *sprite_sheet_cells, context sprite_selection_cursor)
{
  SDL_SetRenderDrawColor(renderer, 74, 50, 110, 255);
  SDL_RenderClear(renderer);


  context_render(&sprite_canvas_ctx);
  context_render(&color_picker_ctx);

  char i;
  for (i = 0; i < SPRITESHEET_SIZE; i++)
  {
    context_render(sprite_sheet_cells + i);
  }

#ifdef __DEBUG_SELECT_WINDOW__
  context_render(&select_window_ctx);
#endif
  context_render(&sprite_selection_cursor);

  SDL_RenderPresent(renderer);
}

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