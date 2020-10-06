CFLAGS   = -g `sdl2-config --cflags`
LDFLAGS  = `sdl2-config --libs` -lSDL2 -lSDL2_ttf -lm
PROG = spritely
CC = gcc

SRCS = $(addprefix src/, main.c spritely.c input.c init.c util.c context.c file.c message_queue.c globals.c colors.c sfd.c)
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
	-s ERROR_ON_UNDEFINED_SYMBOLS=0 -s LLD_REPORT_UNDEFINED -o web/index.js

.PHONY: web
