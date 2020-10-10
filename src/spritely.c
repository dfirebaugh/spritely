#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "globals.h"
#include "input.h"

unsigned int spritely_initialized = 0;

static void toolbar_render(Context_t ctx)
{
  color_t pixel_buffer[COLORPICKER_PIXEL_SIZE];

  unsigned int i;

  for (i = 0; i < TOOLBAR_ROW_SIZE; i++)
  {
    Context_render_icon(toolbar_ctx, icon_files[i], i);
  }
}

static void color_picker_init(Context_t ctx)
{
  color_t pixel_buffer[COLORPICKER_PIXEL_SIZE];

  unsigned int i;

  for (i = 0; i < (COLORPICKER_ROW_SIZE * COLORPICKER_ROW_SIZE); i++)
    pixel_buffer[i] = i;

  Context_from_pixel_buffer(ctx, pixel_buffer);
}

#define SPRITE_SELECTOR_CELL_X_PADDING 0
#define SPRITE_SELECTOR_CELL_Y_PADDING 0

static void sprite_selector_init(Context_t *ctx)
{
  unsigned int i, j;
  unsigned int index = 0;
  for (i = 0; i < SPRITESHEET_COL_SIZE; i++)
  {
    for (j = 0; j < SPRITESHEET_ROW_SIZE; j++)
    {
      int sprite_height = SPRITESHEET_PIXEL_SIZE * 8;
      int sprite_y_pos = sprite_height * i;

      int x = (j * (SPRITESHEET_PIXEL_SIZE * 8) + j) + j;
      int y = (SPRITESHEET_YOFFSET + sprite_y_pos) + i;

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
  Context_render(toolbar_ctx);

  unsigned int i;
  for (i = 0; i < SPRITESHEET_SIZE; i++)
  {
    Context_render(sprite_selector_cells[i]);
  }
  Context_render(sprite_selector_ctx);
  toolbar_render(toolbar_ctx);


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
    pen_color = BLUE;
    sprite_canvas_ctx = Context_make(SPRITE_CANVAS_PIXEL_SIZE,
                                     SPRITE_CANVAS_ROW_SIZE, SPRITE_CANVAS_ROW_SIZE, 0, 0);
    sprite_selector_ctx = Context_make(SPRITESHEET_CELL_SIZE,
                                       SPRITESHEET_ROW_SIZE, SPRITESHEET_COL_SIZE, 0, SPRITESHEET_YOFFSET);
    color_picker_ctx = Context_make(COLORPICKER_PIXEL_SIZE,
                                    COLORPICKER_ROW_SIZE, COLORPICKER_ROW_SIZE,
                                    COLORPICKER_XOFFSET, COLORPICKER_YOFFSET);

    toolbar_ctx = Context_make(COLORPICKER_PIXEL_SIZE, TOOLBAR_ROW_SIZE, TOOLBAR_COLUMN_SIZE, TOOLBAR_XOFFSET, TOOLBAR_YOFFSET);

    sprite_selector_init(sprite_selector_cells);
    color_picker_init(color_picker_ctx);

    Context_make_indicator(color_picker_ctx);
    Context_make_indicator(sprite_selector_ctx);
    Context_make_indicator(toolbar_ctx);

    Context_make_transparent(sprite_selector_ctx);
    Context_make_transparent(toolbar_ctx);

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
