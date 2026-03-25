if (param_get("wayland")) {
    use_backend("wayland", "x11");
}
else {
    use_backend("x11");
}

1;
