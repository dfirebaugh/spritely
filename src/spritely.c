#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "globals.h"
#include "input.h"

char spritely_initialized = 0;

static void color_picker_init(Context_t ctx)
{
  color_t pixel_buffer[COLORPICKER_PIXEL_SIZE];

  char i;

  for (i = 0; i < (COLORPICKER_ROW_SIZE * COLORPICKER_ROW_SIZE); i++)
    pixel_buffer[i] = i;

  Context_from_pixel_buffer(ctx, pixel_buffer);
}

#define SPRITE_SELECTOR_CELL_X_PADDING 6
#define SPRITE_SELECTOR_CELL_Y_PADDING 2

static void sprite_selector_init(Context_t *ctx)
{
  char i, j;
  char index = 0;
  for (i = 0; i < SPRITESHEET_COL_SIZE; i++)
  {
    for (j = 0; j < SPRITESHEET_ROW_SIZE; j++)
    {
      int x = 1 + (j * SPRITESHEET_PIXEL_SIZE * (36 / SPRITESHEET_PIXEL_SIZE) + j) + SPRITE_SELECTOR_CELL_X_PADDING * j;
      int y = SPRITESHEET_YOFFSET + ((SPRITESHEET_PIXEL_SIZE * (SPRITE_CANVAS_SIZE / 8)) * i) + SPRITE_SELECTOR_CELL_Y_PADDING * i;

      ctx[index] = Context_make(SPRITESHEET_PIXEL_SIZE,
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

  Context_render(sprite_canvas_ctx);
  Context_render(color_picker_ctx);

  char i;
  for (i = 0; i < SPRITESHEET_SIZE; i++)
  {
    Context_render(sprite_selector_cells[i]);
  }

#ifdef __DEBUG_SPRITE_SELECTOR__
  Context_render(&sprite_selector_ctx);
#endif
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &sprite_selection_indicator);
  SDL_RenderDrawRect(renderer, &color_picker_indicator);

  Message_box_render(command_message_queue);
  Message_box_render(help_message_queue);
  SDL_RenderPresent(renderer);
}

void emscripten_loop(void *arg)
{
  process_inputs();
  render();
}

void spritely_run()
{
  if (!spritely_initialized)
  {
    // read_file();

    pen_color = BLUE;
    sprite_canvas_ctx = Context_make(SPRITE_CANVAS_PIXEL_SIZE,
                                     SPRITE_CANVAS_ROW_SIZE, SPRITE_CANVAS_ROW_SIZE, 0, 0);
    sprite_selector_ctx = Context_make(SPRITESHEET_CELL_SIZE,
                                       SPRITESHEET_ROW_SIZE, SPRITESHEET_COL_SIZE, 0, SPRITESHEET_YOFFSET);
    color_picker_ctx = Context_make(COLORPICKER_PIXEL_SIZE,
                                    COLORPICKER_ROW_SIZE, COLORPICKER_ROW_SIZE,
                                    COLORPICKER_XOFFSET, COLORPICKER_YOFFSET);

    sprite_selector_init(sprite_selector_cells);
    color_picker_init(color_picker_ctx);

    Context_indicator_focus(&sprite_selection_indicator, sprite_selector_ctx, 0);

    command_message_queue = Message_Queue_create(10);
    help_message_queue = Message_Queue_create(10);

    spritely_initialized = 1;
  }

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(emscripten_loop, NULL, -1, 1);
#else
  while (1)
  {
    process_inputs();

    render();
  }
#endif
}
