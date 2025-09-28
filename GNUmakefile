# $Id$

TARGET = $(shell uname -s)

CC = gcc
CFLAGS = -fPIC -Iinclude -D_MILSKO
LDFLAGS =
LIBS =

L_OBJS =

ifeq ($(TARGET),NetBSD)
CFLAGS += -I/usr/X11R7/include -I/usr/pkg/include
LDFLAGS += -L/usr/X11R7/lib -L/usr/pkg/lib
UNIX = 1
else ifeq ($(TARGET),Linux)
UNIX = 1
else
$(error Add your platform definition)
endif

ifeq ($(UNIX),1)
CFLAGS += -DUSE_X11
LIB = lib
SO = .so
L_OBJS += src/x11.o
L_LIBS += -lX11
endif

.PHONY: all clean
.SUFFIXES: .c .o

all: $(LIB)milsko$(SO)

$(LIB)milsko$(SO): $(L_OBJS)
	$(CC) $(LDFLAGS) -shared -o $@ $(L_OBJS) $(L_LIBS)

clean:
	rm -f *.dll *.so *.a */*.o
