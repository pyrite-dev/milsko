# $Id$

PREFIX = /usr/local

USE_STB_IMAGE = 1
USE_STB_TRUETYPE = 0
USE_FREETYPE2 = 1

CC = $(GCC)gcc

CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -Wno-sign-compare -Iinclude
LDFLAGS =
LIBS =

ifeq ($(DEBUG),1)
CFLAGS += -g
else
CFLAGS += -O2
endif

L_CFLAGS = $(DEPINC) $(CFLAGS) -fPIC -D_MILSKO
L_LDFLAGS = $(LDFLAGS)
L_LIBS = $(LIBS)

L_OBJS = src/core.o src/default.o src/draw.o src/lowlevel.o src/error.o src/unicode.o src/color.o src/messagebox.o src/directory.o src/string.o src/filechooser.o
L_OBJS += external/stb_ds.o external/stb_image.o external/stb_truetype.o
L_OBJS += src/widget/window.o src/widget/button.o src/widget/frame.o src/widget/menu.o src/widget/submenu.o src/widget/image.o src/widget/scrollbar.o src/widget/checkbox.o src/widget/label.o src/widget/entry.o src/widget/numberentry.o src/widget/viewport.o src/widget/listbox.o
L_OBJS += src/cursor/hidden.o src/cursor/default.o src/cursor/cross.o src/cursor/text.o
L_OBJS += src/icon/warning.o src/icon/note.o src/icon/info.o src/icon/news.o src/icon/error.o src/icon/file.o src/icon/directory.o src/icon/back.o src/icon/forward.o src/icon/up.o src/icon/down.o src/icon/left.o src/icon/right.o src/icon/computer.o src/icon/search.o
L_OBJS += src/text/font.o src/text/boldfont.o src/text/ttf.o src/text/boldttf.o src/text/draw.o

E_CFLAGS = $(CFLAGS)
E_LDFLAGS = $(LDFLAGS) -Lsrc -Wl,-rpath,$(shell pwd)/src
E_LIBS = $(LIBS) -lMw

EXAMPLES = examples/basic/example$(EXEC) examples/basic/rotate$(EXEC) examples/basic/image$(EXEC) examples/basic/scrollbar$(EXEC) examples/basic/checkbox$(EXEC) examples/basic/messagebox$(EXEC) examples/basic/viewport$(EXEC) examples/basic/listbox$(EXEC) examples/color_picker$(EXEC)

include mk/platform.mk
include mk/flags.mk
include mk/stb.mk
include mk/freetype2.mk
include mk/opengl.mk
include mk/vulkan.mk

.PHONY: all install format clean lib examples

all: lib examples
lib: src/$(LIB)Mw$(SO)
examples: $(EXAMPLES)

install: lib
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/share/doc/milsko
	for i in src; do \
		cp $$i/*.so $(PREFIX)/lib/ ; \
		cp $$i/*.a $(PREFIX)/lib/ ; \
		cp $$i/*.dll $(PREFIX)/bin/ ; \
	done ; true
	cp -rf include/Mw $(PREFIX)/include/
	cp -rf doc/* $(PREFIX)/share/doc/milsko/

format:
	clang-format --verbose -i `find src include examples tools "(" -name "*.c" -or -name "*.h" ")" -and -not -name "stb_*.h" -and -not -name "ttf.c" -and -not -name "boldttf.c" -and -not -name "font.c" -and -not -name "boldfont.c"`
	perltidy -b -bext='/' --paren-tightness=2 `find tools -name "*.pl"`

src/$(LIB)Mw$(SO): $(L_OBJS)
	$(CC) $(L_LDFLAGS) $(SHARED) -o $@ $^ $(L_LIBS)

examples/gldemos/%$(EXEC): examples/gldemos/%.o src/$(LIB)Mw$(SO)
	$(CC) $(E_LDFLAGS) -o $@ $< $(E_LIBS) $(GL)

examples/%$(EXEC): examples/%.o src/$(LIB)Mw$(SO)
	$(CC) $(E_LDFLAGS) -o $@ $< $(E_LIBS)

src/%.o: src/%.c
	$(CC) $(L_CFLAGS) -c -o $@ $<

external/%.o: external/%.c
	$(CC) $(L_CFLAGS) -Wno-unused-value -Wno-unused-parameter -Wno-unused-function -Wno-stringop-overflow -c -o $@ $<

examples/%.o: examples/%.c
	$(CC) $(E_CFLAGS) -c -o $@ $<

clean:
	rm -f */*.dll */*.so */*.lib */*.a */*.o */*/*.o external/*/src/*.o examples/*.exe examples/*/*.exe $(EXAMPLES)
