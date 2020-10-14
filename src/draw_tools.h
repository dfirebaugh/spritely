#ifndef _DRAWTOOLS
#define _DRAWTOOLS


extern void draw_tool_handle_open_file();
extern void draw_tool_handle_undo();
extern void draw_tool_handle_redo();
extern void draw_tool_activate_fill();
extern void draw_tool_activate_pen();
extern void decrement_row_sprite_selector();
extern void increment_row_sprite_selector();
extern void decrement_sprite_selector();
extern void increment_sprite_selector();
extern void copy_sprite();
extern void paste_sprite();
extern void right_clicks();
extern void left_clicks();
extern void show_help();
extern void tool_sprite_selection(const unsigned int rect_index);

#endif