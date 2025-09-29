# $Id$

TARGET = $(shell uname -s)

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS =
LIBS =

L_CFLAGS = $(CFLAGS) -fPIC -D_MILSKO
L_LDFLAGS = $(LDFLAGS)
L_LIBS = $(LIBS)

E_CFLAGS = $(CFLAGS)
E_LDFLAGS = $(LDFLAGS) -Lsrc
E_LIBS = $(LIBS) -lMw

L_OBJS = src/ds.o src/core.o src/default.o src/draw.o src/lowlevel.o src/font.o
L_OBJS += src/window.o src/button.o

ifeq ($(TARGET),NetBSD)
L_CFLAGS += -I/usr/X11R7/include -I/usr/pkg/include
L_LDFLAGS += -L/usr/X11R7/lib -L/usr/pkg/lib -Wl,-R/usr/X11R7/lib -Wl,-R/usr/pkg/lib
UNIX = 1
else ifeq ($(TARGET),Linux)
UNIX = 1
else ifeq ($(TARGET),Windows)
WINDOWS = 1
else
$(error Add your platform definition)
endif

ifeq ($(UNIX),1)
L_CFLAGS += -DUSE_X11
L_OBJS += src/x11.o
L_LIBS += -lX11

SO = .so
EXEC =
else ifeq ($(WINDOWS),1)
L_CFLAGS += -DUSE_GDI
L_LDFLAGS += -Wl,--out-implib,src/libMw.lib
L_OBJS += src/gdi.o
L_LIBS += -lgdi32

SO = .dll
EXEC = .exe
endif

EXAMPLES = examples/example$(EXEC)

.PHONY: all format clean lib examples

all: lib examples	
lib: src/libMw$(SO)
examples: $(EXAMPLES)

format:
	clang-format --verbose -i $(shell find src include -name "*.c" -or -name "*.h")

src/libMw$(SO): $(L_OBJS)
	$(CC) $(L_LDFLAGS) -shared -o $@ $(L_OBJS) $(L_LIBS)

examples/%$(EXEC): examples/%.o src/libMw$(SO)
	$(CC) $(E_LDFLAGS) -o $@ $< $(E_LIBS)

src/%.o: src/%.c
	$(CC) $(L_CFLAGS) -c -o $@ $<

examples/%.o: examples/%.o
	$(CC) $(E_CFLAGS) -c -o $@ $<

clean:
	rm -f src/*.dll src/*.so src/*.a src/*.lib */*.o examples/*.exe $(EXAMPLES)
