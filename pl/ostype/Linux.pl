my @enabled_backends = ();

if (param_get("wayland")) {
    push(@enabled_backends, "wayland");
}
if (param_get("x11")) {
    push(@enabled_backends, "x11");
}

use_backend(@enabled_backends);

1;
