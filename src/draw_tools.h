#ifndef _DRAWTOOLS
#define _DRAWTOOLS

typedef enum draw_event {
  ACTIVATE_PEN = 0,
  ACTIVATE_FILL,
  ACTIVATE_DRAG,
  LEFT_CLICK_EVENT,
  RIGHT_CLICK_EVENT,
  COPY_SPRITE,
  PASTE_SPRITE,
  OPEN_FILE,
  SAVE_FILE,
  HANDLE_REDO,
  HANDLE_UNDO,
  LEFT_ARROW,
  RIGHT_ARROW,
  UP_ARROW,
  DOWN_ARROW,
  SHOW_HELP
} draw_event_t;

extern void Draw_tool_handle_event(draw_event_t event);

#endif