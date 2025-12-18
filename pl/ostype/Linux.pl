if (param_get("experimental-wayland")) {
    use_backend("wayland", "x11");
}
else {
    use_backend("x11");
}

1;
