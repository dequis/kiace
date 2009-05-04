CC = gcc

CFLAGS += -W -Wall -g
CFLAGS += `pkg-config --cflags sdl cairo`
LIBS = `pkg-config --libs sdl cairo`

TARGETS = cairoplot
OBJECTS = main.o dx_screen.o dx_events.o dx_getopts.o dx_pcm.o

#-lGL -lGLU

all: $(TARGETS)

cairoplot: $(OBJECTS)
	$(CC) -g -o cairoplot $(OBJECTS) $(CFLAGS) $(LIBS)

clean:
	$(RM) $(TARGETS)
	$(RM) $(OBJECTS)
