# $Id$

ifeq ($(TARGET),)
TARGET = $(shell uname -s)
endif

CC = $(GCC)gcc
CXX = $(GCC)g++

CFLAGS = -Wall -Wextra -Wno-unused-parameter -Wno-implicit-fallthrough -Iinclude
LDFLAGS =
LIBS =

ifeq ($(DEBUG),1)
CFLAGS += -g
endif

ifeq ($(VULKAN_NO_STRING_HELPER),1)
VK_STRING_HELPER_DEFINE =
else
VK_STRING_HELPER_DEFINE = -DHAS_VK_ENUM_STRING_HELPER
endif
CFLAGS += $(VK_STRING_HELPER_DEFINE)

L_CFLAGS = $(DEPINC) $(CFLAGS) -fPIC -D_MILSKO
L_LDFLAGS = $(LDFLAGS)
L_LIBS = $(LIBS)

L_OBJS = src/core.o src/default.o src/draw.o src/lowlevel.o src/font.o src/boldfont.o src/error.o
L_OBJS += src/external/ds.o src/external/image.o
L_OBJS += src/widget/window.o src/widget/button.o src/widget/frame.o src/widget/menu.o src/widget/submenu.o src/widget/image.o src/widget/scrollbar.o src/widget/checkbox.o src/widget/label.o src/widget/text.o
L_OBJS += src/cursor/default.o src/cursor/cross.o src/cursor/text.o

OOL_CXXFLAGS = $(DEPINC) $(CFLAGS) -std=c++98 -fPIC
OOL_LDFLAGS = $(LDFLAGS) -L src
OOL_LIBS = $(LIBS) -lMw

OOL_OBJS = oosrc/base.o
include oosrc/deps.mk

E_CFLAGS = $(CFLAGS)
E_LDFLAGS = $(LDFLAGS) -Lsrc
E_LIBS = $(LIBS) -lMw

FOUND_PLATFORM = 0

ifeq ($(TARGET),NetBSD)
CFLAGS += -I/usr/X11R7/include -I/usr/pkg/include
LDFLAGS += -L/usr/X11R7/lib -L/usr/pkg/lib -Wl,-R/usr/X11R7/lib -Wl,-R/usr/pkg/lib
UNIX = 1
OPENGL = 1
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

# Standard gcc accepts this but we have to override this for Apple's gcc.
SHARED = -shared

ifeq ($(UNIX),1)
L_CFLAGS += -DUSE_X11 -DUNIX
L_OBJS += src/backend/x11.o
L_LIBS += -lm -lX11 -lXrender -lXext -lXcursor

GL = -lGL -lGLU

E_LIBS += -lm

LIB = lib
SO = .so
EXEC =
endif

ifeq ($(WINDOWS),1)
L_CFLAGS += -DUSE_GDI
L_LDFLAGS += -Wl,--out-implib,src/libMw.a -static-libgcc
L_OBJS += src/backend/gdi.o
L_LIBS += -lgdi32

GL = -lopengl32 -lglu32

LIB =
SO = .dll
EXEC = .exe
endif

ifeq ($(DARWIN),1)
L_CFLAGS += -DSTBI_NO_THREAD_LOCALS -DUSE_DARWIN
L_OBJS += src/backend/mac/mac.o src/backend/mac/carbon.o

LIB = lib
SO = .dylib
EXEC = 

SHARED = -dynamiclib
endif

ifeq ($(NO_STB_IMAGE),1)
include external/deps.mk
else
L_CFLAGS += -DUSE_STB_IMAGE
endif

EXAMPLES = examples/example$(EXEC) examples/rotate$(EXEC) examples/image$(EXEC) examples/scrollbar$(EXEC) examples/checkbox$(EXEC)

ifeq ($(OPENGL),1)
L_OBJS += src/widget/opengl.o
OOL_OBJS += oosrc/widget/opengl.o
EXAMPLES += examples/gltriangle$(EXEC) examples/glgears$(EXEC) examples/glboing$(EXEC) examples/glcube$(EXEC)
endif

ifeq ($(VULKAN),1)
L_OBJS += src/widget/vulkan.o
OOL_OBJS += oosrc/widget/vulkan.o
EXAMPLES += examples/vulkan$(EXEC)
endif

.PHONY: all format clean lib oolib examples

all: lib examples
lib: src/$(LIB)Mw$(SO)
oolib: oosrc/$(LIB)MwOO$(SO)
examples: $(EXAMPLES)

format:
	clang-format --verbose -i `find oosrc src include examples tools "(" -name "*.c" -or -name "*.h" ")" -and -not -name "stb_*.h"`
	perltidy -b -bext='/' --paren-tightness=2 `find tools -name "*.pl"`

src/$(LIB)Mw$(SO): $(L_OBJS)
	$(CC) $(L_LDFLAGS) $(SHARED) -o $@ $^ $(L_LIBS)

oosrc/$(LIB)MwOO$(SO): $(OOL_OBJS) src/$(LIB)Mw$(SO)
	$(CC) $(OOL_LDFLAGS) $(SHARED) -o $@ $(OOL_OBJS) $(OOL_LIBS)

examples/gl%$(EXEC): examples/gl%.o src/$(LIB)Mw$(SO)
	$(CC) $(E_LDFLAGS) -o $@ $< $(E_LIBS) $(GL)

examples/%$(EXEC): examples/%.o src/$(LIB)Mw$(SO)
	$(CC) $(E_LDFLAGS) -o $@ $< $(E_LIBS)

src/%.o: src/%.c
	$(CC) $(L_CFLAGS) -c -o $@ $<

oosrc/%.o: oosrc/%.cc
	$(CXX) $(OOL_CXXFLAGS) -fno-exceptions -fno-rtti -c -o $@ $<

external/%.o: external/%.c
	$(CC) $(L_CFLAGS) -c -o $@ $<

examples/%.o: examples/%.c
	$(CC) $(E_CFLAGS) -c -o $@ $<

clean:
	rm -f */*.dll */*.so */*.a */*.o */*/*.o external/*/src/*.o examples/*.exe $(EXAMPLES) examples/opengl examples/vulkan
