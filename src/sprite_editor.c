#include "globals.h"

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

void sprite_editor_render()
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

//   Sprite_sheet_render_sprite(main_sprite_sheet, 1, 0, 0);

  Message_box_render(command_message_queue);
  Message_box_render(help_message_queue);
}

void sprite_editor_init()
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

  /* set the color picker to the current active pen_color */
  Context_indicator_focus(color_picker_ctx, pen_color);

  Context_make_transparent(sprite_selector_ctx);
  Context_make_transparent(toolbar_ctx);

//   main_sprite_sheet = Sprite_sheet_make("test.png");

  command_message_queue = Message_Queue_create(10);
  help_message_queue = Message_Queue_create(10);
  spritely_editor_initialized = 1;
}

static void free_all_contexts()
{

    Context_free(sprite_canvas_ctx);
    Context_free(sprite_selector_ctx);
    Context_free(color_picker_ctx);

    unsigned int i, j;
    unsigned int index = 0;
    for (i = 0; i < SPRITESHEET_COL_SIZE; i++)
    {
        for (j = 0; j < SPRITESHEET_ROW_SIZE; j++)
        {
            Context_free(sprite_selector_cells[index]);
            index++;
        }
    }
}

static void free_all()
{
    free_all_contexts();
    Message_Queue_free(command_message_queue);
    Message_Queue_free(help_message_queue);
    Sprite_sheet_free(main_sprite_sheet);
    App_State_free(spritely_state);
}

void sprite_editor_inputs(SDL_Event event)
{
    switch (event.type)
    {
    /* Closing the Window or pressing Escape will exit the program */
    case SDL_QUIT:
        free_all();
        exit(0);
        break;
    case SDL_MOUSEMOTION:
        mouse.x = event.motion.x;
        mouse.y = event.motion.y;
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            Draw_tool_handle_event(LEFT_CLICK_EVENT);
            break;
        case SDL_BUTTON_RIGHT:
            Draw_tool_handle_event(RIGHT_CLICK_EVENT);
            break;
        case SDL_BUTTON_X1:
            /* for some reason right mouse clicks are registering as SDL_BUTTON_X1 but only when I am moving the mouse.... ??????*/
            Draw_tool_handle_event(RIGHT_CLICK_EVENT);
            break;
        default:
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            Draw_tool_handle_event(LEFT_CLICK_EVENT);
            break;
        case SDL_BUTTON_RIGHT:
            Draw_tool_handle_event(RIGHT_CLICK_EVENT);
            break;
        default:
            break;
        }
        break;

    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            free_all();
            exit(0);
            break;
        case SDLK_s:
            if (lctrl)
            {
                Draw_tool_handle_event(SAVE_FILE);
            }
            break;
        case SDLK_o:
            if (lctrl)
            {
                Draw_tool_handle_event(OPEN_FILE);
            }
            break;
        case SDLK_F1:
            Draw_tool_handle_event(SHOW_HELP);
            break;
        case SDLK_LCTRL:
            lctrl = 1;
            break;
        case SDLK_LSHIFT:
            lshift = 1;
            break;
        case SDLK_c:
            if (lctrl)
                Draw_tool_handle_event(COPY_SPRITE);
            break;
        case SDLK_v:
            if (lctrl)
                Draw_tool_handle_event(PASTE_SPRITE);
            break;
        case SDLK_y:
            if (lctrl)
                Draw_tool_handle_event(HANDLE_REDO);
            break;
        case SDLK_z:
            if (lctrl && !lshift)
                Draw_tool_handle_event(HANDLE_UNDO);
            else if (lctrl && lshift)
                Draw_tool_handle_event(HANDLE_REDO);
        case SDLK_f:
            Draw_tool_handle_event(ACTIVATE_FILL);
            break;
        case SDLK_SPACE:
            Draw_tool_handle_event(ACTIVATE_PEN);
            break;
        case SDLK_LEFT:
            Draw_tool_handle_event(LEFT_ARROW);
            break;
        case SDLK_RIGHT:
            Draw_tool_handle_event(RIGHT_ARROW);
            break;
        case SDLK_DOWN:
            Draw_tool_handle_event(DOWN_ARROW);
            break;
        case SDLK_UP:
            Draw_tool_handle_event(UP_ARROW);
            break;

        default:
            break;
        }
        break;

    case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
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
