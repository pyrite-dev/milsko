use_backend("classicmacos");

$cc = $ENV{RETRO68_TOOLCHAIN_PATH}."/bin/m68k-apple-macos-gcc";
$ar = $ENV{RETRO68_TOOLCHAIN_PATH}."/bin/m68k-apple-macos-ar";
add_cflags("-DCLASSIC_MAC_OS");

1;
