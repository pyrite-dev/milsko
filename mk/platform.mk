# $Id$

ifeq ($(TARGET),)
TARGET = $(shell uname -s)
endif
ifneq ($(TARGET),$(shell uname -s))
CROSS = 1
endif

FOUND_PLATFORM = 0

ifeq ($(TARGET),NetBSD)
CFLAGS += -I/usr/X11R7/include -I/usr/pkg/include
LDFLAGS += -L/usr/X11R7/lib -L/usr/pkg/lib -Wl,-R/usr/X11R7/lib -Wl,-R/usr/pkg/lib
UNIX = 1
OPENGL = 1
VULKAN = 1
FOUND_PLATFORM = 1
endif

ifeq ($(TARGET),Linux)
L_LIBS += -ldl
UNIX = 1
OPENGL = 1
VULKAN = 1
FOUND_PLATFORM = 1
endif

ifeq ($(TARGET),Windows)
CFLAGS += -mmmx

WINDOWS = 1
OPENGL = 1
VULKAN = 1
FOUND_PLATFORM = 1
endif

ifeq ($(TARGET),SunOS)
CC = gcc
UNIX = 1
L_LIBS += -lsocket -lnsl
OPENGL = 1
FOUND_PLATFORM = 1
endif

ifeq ($(TARGET),Darwin)
CC = gcc
DARWIN = 1
L_LIBS += -framework Carbon
FOUND_PLATFORM = 1
endif

ifeq ($(FOUND_PLATFORM),0)
$(error Add your platform definition)
endif
