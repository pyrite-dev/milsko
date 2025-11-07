dnl $Id$
define([executable_suffix],[.exe])dnl
define([library_prefix],[])dnl
define([library_suffix],[.dll])dnl
add_ldflag([-Wl,--out-implib,src/libMw.a -static-libgcc])dnl
define([math],[])dnl
define([use_gdi])dnl
