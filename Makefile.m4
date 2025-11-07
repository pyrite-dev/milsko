dnl $Id$
dnl
dnl This is toplevel Makefile.m4.
dnl
dnl Notes:
dnl  1. add_cflag/add_ldflag/add_libs only affects Milsko library
dnl  2. add_incdir/add_libdir affects globally
dnl
include(m4/toplevel/rules.m4)dnl
dnl
new_object([src/*.c])dnl
dnl
new_object([src/icon/*.c])dnl
new_object([src/font/*.c])dnl
new_object([src/cursor/*.c])dnl
dnl
new_object([src/widget/button.c])dnl
new_object([src/widget/checkbox.c])dnl
new_object([src/widget/entry.c])dnl
new_object([src/widget/frame.c])dnl
new_object([src/widget/image.c])dnl
new_object([src/widget/label.c])dnl
new_object([src/widget/listbox.c])dnl
new_object([src/widget/menu.c])dnl
new_object([src/widget/numberentry.c])dnl
new_object([src/widget/progressbar.c])dnl
new_object([src/widget/radiobox.c])dnl
new_object([src/widget/scrollbar.c])dnl
new_object([src/widget/submenu.c])dnl
new_object([src/widget/viewport.c])dnl
new_object([src/widget/window.c])dnl
dnl
new_object([src/dialog/*.c])dnl
dnl
new_object([src/abstract/*.c])dnl
dnl
new_object([external/*.c])dnl
dnl
ifdef([target],[],[define([target],esyscmd([uname -s | xargs printf '%s']))])dnl
syscmd([test -f ]m4/ostype/target.m4)dnl
ifelse(sysval,[0],[include(m4/ostype/target.m4)],[errprint([M4 for your target (]m4/ostype/target.m4[) was not found, please make one
])m4exit(1)dnl
])dnl
dnl
include(m4/toplevel/options.m4)dnl
dnl
new_example([examples/basic/example])dnl
new_example([examples/basic/rotate])dnl
new_example([examples/basic/image])dnl
new_example([examples/basic/scrollbar])dnl
new_example([examples/basic/checkbox])dnl
new_example([examples/basic/radiobox])dnl
new_example([examples/basic/messagebox])dnl
new_example([examples/basic/viewport])dnl
new_example([examples/basic/listbox])dnl
new_example([examples/basic/progressbar])dnl
new_example([examples/basic/colorpicker])dnl
dnl
CC = cc
CFLAGS = cflags
LDFLAGS = ldflags
INCDIR = incdir
LIBDIR = libdir
LIBS = libs
MATH = math
SHARED = shared_flag

.PHONY: all clean distclean lib examples install

all: lib examples

pushdef([library])dnl
define([library],[src/library_prefix[]Mw[]library_suffix])dnl
lib: library

library: library_targets
	$(CC) $(LDFLAGS) $(LIBDIR) $(SHARED) -o library library_targets $(LIBS)

print_library_targets()dnl

examples: examples_targets

print_examples_targets()dnl
popdef([library])dnl

distclean: clean
	rm -f Makefile

clean:
	rm -f examples_targets */*.o */*/*.o */*/*/*.o src/*.dll src/*.so src/*.a
