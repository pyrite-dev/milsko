# $Id$

if(system("wayland-scanner -v >/dev/null 2>&1") == 0){
    use_backend("wayland", "x11");
} else {
    use_backend("x11");
}

1;
