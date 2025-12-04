CC = gcc
CFLAGS = `freetype-config --cflags`
LDFLAGS =
LIBS = `freetype-config --libs`

.PHONY: all clean
.SUFFIXES: .c .o

all: font

font: font.o
	$(CC) $(LDFLAGS) -o $@ font.o $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o font
