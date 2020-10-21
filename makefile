PY_CFLAGS  := $(shell python3.9-config --cflags)
PY_LDFLAGS := $(shell python3.9-config --ldflags --embed)
CFLAGS   = -g `sdl2-config --cflags` $(PY_CFLAGS) -fPIE -Wall -Werror --pedantic
LDFLAGS  = `sdl2-config --libs` -lSDL2 -lSDL2_ttf -lSDL2_image -lm $(PY_LDFLAGS) 
PROG = spritely
CC = gcc

SRCS = $(addprefix src/, main.c spritely.c init.c util.c context.c file.c \
message_queue.c globals.c colors.c sfd.c draw_tools.c sprite_sheet.c app_state.c \
sprite_editor.c shell.c python_api.c entity.c js_api.c)
OBJS = $(SRCS:.c=.o)

# top-level rule to create the program.
all: $(PROG)

# linking the program
$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(LDFLAGS)

# cleaning everything that can be automatically recreated with "make"
clean:
	rm $(PROG) $(OBJS)

clean-web:
	rm web/index.js web/index.wasm

web:
	emcc $(SRCS) -s WASM=1 -s USE_SDL=2 -s USE_SDL_TTF=2 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_IMAGE=2 --use-preload-plugins --preload-file assets --preload-file fonts \
	-s ERROR_ON_UNDEFINED_SYMBOLS=0 -s LLD_REPORT_UNDEFINED \
	-s EXPORTED_FUNCTIONS='["_print_something", "_is_btn_pressed", "_render_rect", "_render_rect_fill", "_set_render_color", "_render_char", "_change_state", "_new_entity_manager", "_Entity_free", "_add_sprite_to_entity_manager", "_clear", "_register_draw_fn", "_register_update_fn", "_main"]' \
	-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
	-s RESERVED_FUNCTION_POINTERS=10 \
	-s "EXTRA_EXPORTED_RUNTIME_METHODS=['addFunction']" \
	-s EXPORT_NAME="'MyEmscriptenModule'" -s EXPORT_ES6=1 -s MODULARIZE=1 -s ALLOW_TABLE_GROWTH=1 \
	-o web/index.js -Os && cp web/index* web/examples/

.PHONY: web
