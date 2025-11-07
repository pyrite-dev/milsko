dnl $Id$
add_incdir([-I/usr/X11R7/include -I/usr/pkg/include])dnl
add_libdir([-L/usr/X11R7/lib -Wl,-R/usr/X11R7/lib])dnl
define([use_x11])dnl
