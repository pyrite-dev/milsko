CC = gcc
CFLAGS = `pkg-config --cflags freetype2` -I../include
LDFLAGS =
LIBS = `pkg-config --libs freetype2`

.PHONY: all clean
.SUFFIXES: .c .o

all: fuzzer font

fuzzer: fuzzer.o
	$(CC) -g -L../src/ -Wl,-R../src -o $@ fuzzer.o -lMw

font: font.o
	$(CC) $(LDFLAGS) -o $@ font.o $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o font fuzzer
