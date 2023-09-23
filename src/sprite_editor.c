#include "defs.h"
#include "draw_tools.h"
#include "file.h"
#include "globals.h"

static void toolbar_render(Context_t ctx) {
  uint8_t i;

  static Sprite_sheet_t icon_sprite_sheet = NULL;

  if (!icon_sprite_sheet)
    icon_sprite_sheet = Sprite_sheet_make("assets/icons/icons.png");

  for (i = 0; i < TOOLBAR_ROW_SIZE; i++) {
    Context_render_sprite_in_context_scale(toolbar_ctx, icon_sprite_sheet, i, i,
                                           6);
  }
}

/**
 * @brief Load color palette file
 *
 * @return int 1 if success, 0: if failed
 */
static int load_palette_file(void) {
  char buf[16];
  uint8_t colors[COLOR_VALUES_SIZE];
  FILE *file = fopen(palette_file, "rb");
  if (NULL != file) {
    const int palette_size = COLORPICKER_NUM_COLORS;
    int color_idx = 0;

    for (int i = 0; i < palette_size && !feof(file); ++i) {
      if (fgets(buf, sizeof(buf), file) != NULL) {
        unsigned int r, g, b;
        sscanf(buf, "%u,%u,%u\n", &r, &g, &b);
        colors[color_idx++] = r;
        colors[color_idx++] = g;
        colors[color_idx++] = b;
        colors[color_idx++] = 255;
      } else {
        return 0;
        break;
      }
    }
    fclose(file);
    /* overwrite the default color palette */
    memcpy(color_values, colors, COLOR_VALUES_SIZE);
    return 1;
  }

  return 0;
}

static void color_picker_init(Context_t ctx) {
  color_t pixel_buffer[COLORPICKER_PIXEL_SIZE];

  uint8_t i;

  load_palette_file();

  for (i = 0; i < (COLORPICKER_ROW_SIZE * COLORPICKER_ROW_SIZE); i++)
    pixel_buffer[i] = i;

  Context_from_pixel_buffer(ctx, pixel_buffer);
}

#define SPRITE_SELECTOR_CELL_X_PADDING 0
#define SPRITE_SELECTOR_CELL_Y_PADDING 0

static void sprite_selector_init(Context_t *ctx) {
  uint8_t i, j;
  uint8_t index = 0;
  for (i = 0; i < SPRITESHEET_COL_SIZE; i++) {
    for (j = 0; j < SPRITESHEET_ROW_SIZE; j++) {
      int sprite_height = SPRITESHEET_PIXEL_SIZE * 8;
      int sprite_y_pos = sprite_height * i;

      int x = (j * (SPRITESHEET_PIXEL_SIZE * 8) + j) + j;
      int y = (SPRITESHEET_YOFFSET + sprite_y_pos) + i;

      Context_config_t selector_cell_ctx_config;
      selector_cell_ctx_config.pixel_size = SPRITESHEET_PIXEL_SIZE;
      selector_cell_ctx_config.row_size = SPRITE_CANVAS_ROW_SIZE;
      selector_cell_ctx_config.col_size = SPRITE_CANVAS_ROW_SIZE;
      selector_cell_ctx_config.x_offset = x;
      selector_cell_ctx_config.y_offset = y;
      selector_cell_ctx_config.has_indicator = false;
      selector_cell_ctx_config.is_transparent = false;

      ctx[index] = Context_make_from_config(selector_cell_ctx_config);
      index++;
    }
  }
}

void sprite_editor_render(void) {
  SDL_SetRenderDrawColor(renderer, 74, 50, 110, 255);
  SDL_RenderClear(renderer);

  Context_render(sprite_canvas_ctx);
  Context_render(color_picker_ctx);
  Context_render(toolbar_ctx);

  unsigned int i;
  for (i = 0; i < SPRITESHEET_SIZE; i++) {
    Context_render(sprite_selector_cells[i]);
  }

  Context_render(sprite_selector_ctx);
  toolbar_render(toolbar_ctx);

  Message_box_render(command_message_queue);
  Message_box_render(help_message_queue);
}

int sprite_editor_init(void) {
  pen_color = 0;

  Context_config_t sprite_canvas_ctx_config;
  sprite_canvas_ctx_config.pixel_size = SPRITE_CANVAS_PIXEL_SIZE;
  sprite_canvas_ctx_config.row_size = SPRITE_CANVAS_ROW_SIZE;
  sprite_canvas_ctx_config.col_size = SPRITE_CANVAS_ROW_SIZE;
  sprite_canvas_ctx_config.x_offset = 0;
  sprite_canvas_ctx_config.y_offset = 0;
  sprite_canvas_ctx_config.has_indicator = false;
  sprite_canvas_ctx_config.is_transparent = false;

  Context_config_t color_picker_config;
  color_picker_config.pixel_size = COLORPICKER_PIXEL_SIZE;
  color_picker_config.row_size = COLORPICKER_ROW_SIZE;
  color_picker_config.col_size = COLORPICKER_ROW_SIZE;
  color_picker_config.x_offset = COLORPICKER_XOFFSET;
  color_picker_config.y_offset = COLORPICKER_YOFFSET;
  color_picker_config.has_indicator = true;
  color_picker_config.is_transparent = false;

  Context_config_t toolbar_ctx_config;
  toolbar_ctx_config.pixel_size = COLORPICKER_PIXEL_SIZE;
  toolbar_ctx_config.row_size = TOOLBAR_ROW_SIZE;
  toolbar_ctx_config.col_size = TOOLBAR_COLUMN_SIZE;
  toolbar_ctx_config.x_offset = TOOLBAR_XOFFSET;
  toolbar_ctx_config.y_offset = TOOLBAR_YOFFSET;
  toolbar_ctx_config.has_indicator = true;
  toolbar_ctx_config.is_transparent = true;

  Context_config_t sprite_selector_ctx_config;
  sprite_selector_ctx_config.pixel_size = SPRITESHEET_CELL_SIZE;
  sprite_selector_ctx_config.row_size = SPRITESHEET_ROW_SIZE;
  sprite_selector_ctx_config.col_size = SPRITESHEET_COL_SIZE;
  sprite_selector_ctx_config.x_offset = 0;
  sprite_selector_ctx_config.y_offset = SPRITESHEET_YOFFSET;
  sprite_selector_ctx_config.has_indicator = true;
  sprite_selector_ctx_config.is_transparent = true;

  sprite_canvas_ctx = Context_make_from_config(sprite_canvas_ctx_config);
  color_picker_ctx = Context_make_from_config(color_picker_config);
  toolbar_ctx = Context_make_from_config(toolbar_ctx_config);
  sprite_selector_ctx = Context_make_from_config(sprite_selector_ctx_config);

  sprite_selector_init(sprite_selector_cells);
  color_picker_init(color_picker_ctx);

  /* set the color picker to the current active pen_color */
  Context_indicator_focus(color_picker_ctx, pen_color);
  Context_indicator_focus(toolbar_ctx, 0);
  Context_indicator_focus(sprite_selector_ctx, 0);

  command_message_queue = Message_Queue_create(1);
  help_message_queue = Message_Queue_create(1);
  return 1;
}

static void free_all_contexts(void) {

  Context_free(sprite_canvas_ctx);
  Context_free(sprite_selector_ctx);
  Context_free(color_picker_ctx);

  uint8_t i, j;
  uint8_t index = 0;
  for (i = 0; i < SPRITESHEET_COL_SIZE; i++) {
    for (j = 0; j < SPRITESHEET_ROW_SIZE; j++) {
      Context_free(sprite_selector_cells[index]);
      index++;
    }
  }

  Message_Queue_free(command_message_queue);
  Message_Queue_free(help_message_queue);
}

static void free_all(void) { free_all_contexts(); }

bool mousedown = false;
void sprite_editor_inputs(SDL_Event event) {
  switch (event.type) {
  /* Closing the Window or pressing Escape will exit the program */
  case SDL_QUIT:
    free_all();
    exit(0);
    break;
  case SDL_MOUSEMOTION:
    mouse.x = event.motion.x;
    mouse.y = event.motion.y;
    int canvas_x = (event.motion.x / SPRITE_CANVAS_PIXEL_SIZE);
    int canvas_y = (event.motion.y / SPRITE_CANVAS_PIXEL_SIZE);
    if (canvas_x != mouse.canvas_x || canvas_y != mouse.canvas_y) {
      mouse.canvas_x = canvas_x;
      mouse.canvas_y = canvas_y;
      if (mousedown && event.motion.state & SDL_BUTTON_LMASK)
        left_drags();
    }
  case SDL_MOUSEBUTTONDOWN:
    mousedown = true;
    break;
  case SDL_MOUSEBUTTONUP:
    if (mousedown) {
      switch (event.button.button) {
      case SDL_BUTTON_LEFT:
        left_clicks();
        break;
      case SDL_BUTTON_RIGHT:
        right_clicks();
        break;
      case SDL_BUTTON_X1:
        /* for some reason right mouse clicks are registering as SDL_BUTTON_X1
         * but only when I am moving the mouse.... ??????*/
        right_clicks();
        break;
      default:
        break;
      }
      break;
    }
    mousedown = false;
    break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      free_all();
      exit(0);
      break;
    case SDLK_s:
      if (lctrl) {
        save_file(lshift);
      }
      break;
    case SDLK_o:
      if (lctrl)
        draw_tool_handle_open_file();
      break;
    case SDLK_F1:
      show_help();
      break;
    case SDLK_LCTRL:
      lctrl = 1;
      break;
    case SDLK_LSHIFT:
      lshift = 1;
      break;
    case SDLK_c:
      if (lctrl)
        copy_sprite();
      break;
    case SDLK_v:
      if (lctrl)
        paste_sprite();
      break;
    case SDLK_y:
      if (lctrl)
        draw_tool_handle_redo();
      break;
    case SDLK_z:
      if (lctrl && !lshift)
        draw_tool_handle_undo();
      else if (lctrl && lshift)
        draw_tool_handle_redo();
    case SDLK_f:
      draw_tool_activate_fill();
      break;
    case SDLK_SPACE:
      draw_tool_activate_pen();
      break;
    case SDLK_LEFT:
      decrement_sprite_selector();
      break;
    case SDLK_RIGHT:
      increment_sprite_selector();
      break;
    case SDLK_DOWN:
      increment_row_sprite_selector();
      break;
    case SDLK_UP:
      decrement_row_sprite_selector();
      break;

    default:
      break;
    }
    break;

  case SDL_KEYUP:
    switch (event.key.keysym.sym) {
    case SDLK_LCTRL:
      lctrl = 0;
      break;
    case SDLK_LSHIFT:
      lshift = 0;
      break;

    default:
      break;
    }

  case SDL_USEREVENT:
    break;

  default:
    break;
  }
}
