# $Id$
$library_prefix    = "";
$library_suffix    = ".dll";
$executable_suffix = ".exe";
$math              = "";
add_ldflags("-Wl,--out-implib,src/libMw.a -static-libgcc");
use_backend("gdi");

1;
