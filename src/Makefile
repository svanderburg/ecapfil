CC = gcc -ansi -Wall -pedantic -ggdb
CFLAGS := $(shell pkg-config --cflags sdl2) $(shell pkg-config --cflags SDL2_image)
LIBS := $(shell pkg-config --libs sdl2) $(shell pkg-config --libs SDL2_image)
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

# Enable this line to build on big endian systems
#CFLAGS += -DBUILD_BIG_ENDIAN

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

ecapfil: io.o picanm.o artfile.o vgapalette.o tile.o tilesurface.o tilecatalog.o numericset.o capfilentry.o capfil.o viewer.o main.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

install: ecapfil
	mkdir -p $(BINDIR)
	cp $^ $(BINDIR)

clean:
	rm -f *.o
	rm -f capfil
