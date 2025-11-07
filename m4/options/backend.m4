dnl $Id$
ifdef([use_x11],[add_cflag([-DUSE_X11])dnl
add_lib([-lX11 -lXrender -lXcursor])dnl
new_object([src/backend/x11.c])dnl
define([gl_lib],[-lGL -lGLU])])dnl
dnl
ifdef([use_gdi],[add_cflag([-DUSE_GDI])dnl
add_lib([-lgdi32])dnl
new_object([src/backend/gdi.c])dnl
define([gl_lib],[-lopengl32 -lglu32])])dnl
dnl
ifdef([use_darwin],[add_cflag([-DUSE_DARWIN -DSTBI_NO_THREAD_LOCALS])dnl
new_object([src/backend/mac/*.c])dnl
define([gl_lib],[])])dnl
