# $Id$

TARGET = $(shell uname -s)

CC = gcc
CFLAGS = -Wall -Wextra -fPIC -Iinclude -D_MILSKO -g
LDFLAGS =
LIBS =

L_OBJS = src/ds.o src/core.o src/default.o src/draw.o src/lowlevel.o src/font.o
L_OBJS += src/window.o src/button.o
L_LIBS =

ifeq ($(TARGET),NetBSD)
CFLAGS += -I/usr/X11R7/include -I/usr/pkg/include
LDFLAGS += -L/usr/X11R7/lib -L/usr/pkg/lib -Wl,-R/usr/X11R7/lib -Wl,-R/usr/pkg/lib
UNIX = 1
else ifeq ($(TARGET),Linux)
UNIX = 1
else ifeq ($(TARGET),Windows)
WINDOWS = 1
else
$(error Add your platform definition)
endif

ifeq ($(UNIX),1)
CFLAGS += -DUSE_X11
LIB = lib
SO = .so
L_OBJS += src/x11.o
L_LIBS += -lX11
else ifeq ($(WINDOWS),1)
CFLAGS += -DUSE_GDI
LDFLAGS += -Wl,--out-implib,Mw.lib
LIB =
SO = .dll
L_OBJS += src/gdi.o
L_LIBS += -lgdi32
endif

.PHONY: all format clean
.SUFFIXES: .c .o

all: $(LIB)Mw$(SO)

format:
	clang-format --verbose -i $(shell find src include -name "*.c" -or -name "*.h")

$(LIB)Mw$(SO): $(L_OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $(L_OBJS) $(L_LIBS)

clean:
	rm -f *.dll *.so *.a *.lib */*.o
