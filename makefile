CFLAGS   = -g `sdl2-config --cflags` -I./include -fPIE -Wall -Werror --pedantic
LDFLAGS  = `sdl2-config --libs` -lSDL2 -lSDL2_ttf -lSDL2_image -lm
PROG = spritely
CC = gcc

SRCS = $(addprefix src/,  apprt.c canvas.c color_picker.c color.c editor_tool.c event.c grid_context.c input.c main.c palette.c pixel_buffer.c sprite_editor.c sprite_sheet.c state.c)
OBJS = $(addprefix obj/, $(SRCS:.c=.o))

# top-level rule to create the program.
all: $(PROG)

# linking the program
$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(LDFLAGS)

# rule to create the object files in the obj/ directory
obj/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# cleaning everything that can be automatically recreated with "make"
clean:
	rm -f $(PROG) $(OBJS)
	rm -rf zig-cache
	rm -rf zig-out
	rm -rf obj

clean-web:
	rm -f web/index.js web/index.wasm

web:
	emcc $(SRCS) -I./include -s WASM=1 -s USE_SDL=2 -s USE_SDL_TTF=2 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_IMAGE=2 --use-preload-plugins --preload-file assets --preload-file fonts \
	-s ERROR_ON_UNDEFINED_SYMBOLS=0 -s LLD_REPORT_UNDEFINED \
	-s EXPORTED_FUNCTIONS='["_print_something", "_is_btn_pressed", "_render_rect", "_render_rect_fill", "_set_render_color", "_render_char", "_change_state", "_new_entity_manager", "_Entity_free", "_add_sprite_to_entity_manager", "_clear", "_register_draw_fn", "_register_update_fn", "_main"]' \
	-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","addFunction"]' \
	-s RESERVED_FUNCTION_POINTERS=10 \
	-s EXPORT_NAME="'MyEmscriptenModule'" -s EXPORT_ES6=1 -s MODULARIZE=1 -s ALLOW_TABLE_GROWTH=1 \
	-o web/index.js -Os && cp web/index* web/examples/

.PHONY: web

