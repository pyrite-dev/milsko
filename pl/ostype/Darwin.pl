$library_suffix    = ".dylib";
set_shared_flag("-dynamiclib");

use_backend("cocoa");

add_cflags("-DSTBI_NO_THREAD_LOCALS");
add_ldflags("-framework Cocoa -lobjc");

1;
