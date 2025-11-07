dnl $Id$
include(m4/options/backend.m4)dnl
include(m4/options/opengl.m4)dnl
include(m4/options/vulkan.m4)dnl
include(m4/options/classic.m4)dnl
dnl
ifdef([use_stb_image],[add_cflag([-DUSE_STB_IMAGE])],[new_object([external/libz/src/*.c])dnl
new_object([external/libjpeg/src/*.c])dnl
new_object([external/libpng/src/*.c])dnl
add_cflag([-Iexternal/libz/include])dnl
add_cflag([-Iexternal/libjpeg/include])dnl
add_cflag([-Iexternal/libpng/include])dnl
])dnl
dnl
ifdef([use_stb_truetype],[add_cflag([-DUSE_STB_TRUETYPE])])dnl
ifdef([use_freetype2],[add_cflag([-DUSE_FREETYPE2])dnl
ifdef([cross_build],[],[add_cflag(esyscmd([pkg-config --cflags freetype2 | xargs printf '%s ']))dnl
add_lib(esyscmd([pkg-config --libs freetype2 | xargs printf '%s ']))dnl
])dnl
])dnl
