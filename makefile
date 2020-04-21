CFLAGS   = -g `sdl2-config --cflags`
LDFLAGS  = `sdl2-config --libs` -lSDL2 -lm
PROG = spritely
CXX = gcc

OBJS = main.o spritely.o input.o init.o util.o context.o

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
%.o: src/%.c src/%.h src/defs.h
	$(CXX) $(CFLAGS) -c $<

# linking the program
$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $(PROG) $(LDFLAGS)

# cleaning everything that can be automatically recreated with "make"
clean:
	rm $(PROG) *.o
