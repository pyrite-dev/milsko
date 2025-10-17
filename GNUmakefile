# $Id$

PREFIX = /usr/local

ifeq ($(TARGET),)
TARGET = $(shell uname -s)
endif

USE_STB_IMAGE = 1

CC = $(GCC)gcc
CXX = $(GCC)g++

CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -Wno-sign-compare -Iinclude
LDFLAGS =
LIBS =

ifeq ($(DEBUG),1)
CFLAGS += -g
else
CFLAGS += -O2
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

L_OBJS = src/core.o src/default.o src/draw.o src/lowlevel.o src/font.o src/boldfont.o src/error.o src/unicode.o src/color.o src/messagebox.o src/directory.o src/string.o src/filechooser.o
L_OBJS += external/ds.o external/image.o
L_OBJS += src/widget/window.o src/widget/button.o src/widget/frame.o src/widget/menu.o src/widget/submenu.o src/widget/image.o src/widget/scrollbar.o src/widget/checkbox.o src/widget/label.o src/widget/entry.o src/widget/numberentry.o src/widget/viewport.o src/widget/listbox.o
L_OBJS += src/cursor/default.o src/cursor/cross.o src/cursor/text.o
L_OBJS += src/icon/question.o src/icon/warning.o src/icon/note.o src/icon/info.o src/icon/news.o src/icon/error.o src/icon/file.o src/icon/directory.o src/icon/back.o src/icon/forward.o src/icon/up.o src/icon/computer.o src/icon/search.o

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
L_LIBS += -lm -lX11 -lXrender -lXcursor

GL = -lGL -lGLU

E_LIBS += -lm

LIB = lib
SO = .so
EXEC =
endif

ifeq ($(WINDOWS),1)
L_CFLAGS += -DUSE_GDI
L_LDFLAGS += -Wl,--out-implib,src/libMw.a -static-libgcc
OOL_LDFLAGS += -Wl,--out-implib,oosrc/libMwOO.a -static-libgcc
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

ifeq ($(USE_STB_IMAGE),1)
L_CFLAGS += -DUSE_STB_IMAGE
else
include external/deps.mk
endif

EXAMPLES = examples/basic/example$(EXEC) examples/basic/rotate$(EXEC) examples/basic/image$(EXEC) examples/basic/scrollbar$(EXEC) examples/basic/checkbox$(EXEC) examples/basic/messagebox$(EXEC) examples/basic/viewport$(EXEC) examples/basic/listbox$(EXEC) examples/color_picker$(EXEC)

ifeq ($(OPENGL),1)
L_OBJS += src/widget/opengl.o
OOL_OBJS += oosrc/widget/opengl.o
EXAMPLES += examples/gldemos/clock$(EXEC) examples/gldemos/triangle$(EXEC) examples/gldemos/gears$(EXEC) examples/gldemos/boing$(EXEC) examples/gldemos/cube$(EXEC) examples/gldemos/tripaint$(EXEC)
endif

ifeq ($(VULKAN),1)
L_OBJS += src/widget/vulkan.o
OOL_OBJS += oosrc/widget/vulkan.o
EXAMPLES += examples/vkdemos/vulkan$(EXEC)
endif

.PHONY: all install format clean lib oolib examples

all: lib examples
lib: src/$(LIB)Mw$(SO)
oolib: oosrc/$(LIB)MwOO$(SO)
examples: $(EXAMPLES)

install: lib oolib
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/share/doc/milsko
	for i in src oosrc; do \
		cp $$i/*.so $(PREFIX)/lib/ ; \
		cp $$i/*.a $(PREFIX)/lib/ ; \
		cp $$i/*.dll $(PREFIX)/bin/ ; \
	done ; true
	cp -rf include/Mw $(PREFIX)/include/
	cp -rf include/MwOO $(PREFIX)/include/
	cp -rf doc/* $(PREFIX)/share/doc/milsko/

format:
	clang-format --verbose -i `find oosrc src include examples tools "(" -name "*.c" -or -name "*.h" ")" -and -not -name "stb_*.h"`
	perltidy -b -bext='/' --paren-tightness=2 `find tools -name "*.pl"`

src/$(LIB)Mw$(SO): $(L_OBJS)
	$(CC) $(L_LDFLAGS) $(SHARED) -o $@ $^ $(L_LIBS)

oosrc/$(LIB)MwOO$(SO): $(OOL_OBJS) src/$(LIB)Mw$(SO)
	$(CC) $(OOL_LDFLAGS) $(SHARED) -o $@ $(OOL_OBJS) $(OOL_LIBS)

examples/gldemos/%$(EXEC): examples/gldemos/%.o src/$(LIB)Mw$(SO)
	$(CC) $(E_LDFLAGS) -o $@ $< $(E_LIBS) $(GL)

examples/%$(EXEC): examples/%.o src/$(LIB)Mw$(SO)
	$(CC) $(E_LDFLAGS) -o $@ $< $(E_LIBS)

src/%.o: src/%.c
	$(CC) $(L_CFLAGS) -c -o $@ $<

oosrc/%.o: oosrc/%.cc
	$(CXX) $(OOL_CXXFLAGS) -fno-exceptions -fno-rtti -c -o $@ $<

external/%.o: external/%.c
	$(CC) $(L_CFLAGS) -Wno-unused-value -Wno-unused-parameter -c -o $@ $<

examples/%.o: examples/%.c
	$(CC) $(E_CFLAGS) -c -o $@ $<

clean:
	rm -f */*.dll */*.so */*.lib */*.a */*.o */*/*.o external/*/src/*.o examples/*/*.exe $(EXAMPLES)
