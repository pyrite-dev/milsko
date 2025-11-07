dnl $Id$
ifdef([build_opengl],[new_object([src/widget/opengl.c])dnl
new_example([examples/gldemos/boing],[],[],[gl_lib])dnl
new_example([examples/gldemos/clock],[],[],[gl_lib])dnl
new_example([examples/gldemos/cube],[],[],[gl_lib])dnl
new_example([examples/gldemos/gears],[],[],[gl_lib])dnl
new_example([examples/gldemos/triangle],[],[],[gl_lib])dnl
new_example([examples/gldemos/tripaint],[],[],[gl_lib])dnl
])dnl
