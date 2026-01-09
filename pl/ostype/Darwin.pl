$library_suffix    = ".dylib";
set_shared_flag("-dynamiclib");

use_backend("appkit");

add_cflags("-DSTBI_NO_THREAD_LOCALS");

1;