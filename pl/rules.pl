new_object("src/*.c");

my $gl_libs = "";

if (param_get("classic-theme")) {
    add_cflags("-DUSE_CLASSIC_THEME");
}
if (grep(/^x11$/, @backends)) {
    add_cflags("-DUSE_X11");
    new_object("src/backend/x11.c");
    add_libs("-lX11");

    $gl_libs = "-lGL -lGLU";

    if (param_get("xrender")) {
        add_cflags("-DUSE_XRENDER");
        add_libs("-lXrender");
    }
}

if (grep(/^gdi$/, @backends)) {
    add_cflags("-DUSE_GDI");
    new_object("src/backend/gdi.c");
    add_libs("-lgdi32");

    $gl_libs = "-lopengl32 -lglu32";
}

if (param_get("stb-image")) {
    add_cflags("-DUSE_STB_IMAGE");
}
if (param_get("stb-truetype")) {
    add_cflags("-DUSE_STB_TRUETYPE");
}
if (param_get("freetype2")) {
    add_cflags("-DUSE_FREETYPE2");
    if (not($cross)) {
        add_cflags(`pkg-config --cflags freetype2`);
        add_libs(`pkg-config --libs freetype2`);
    }
}

if (param_get("vulkan") && param_get("vulkan-string-helper")) {
    add_cflags("-DHAS_VK_ENUM_STRING_HELPER");
}

new_object("src/icon/*.c");
new_object("src/font/*.c");
new_object("src/cursor/*.c");

new_object("src/widget/button.c");
new_object("src/widget/checkbox.c");
new_object("src/widget/combobox.c");
new_object("src/widget/entry.c");
new_object("src/widget/frame.c");
new_object("src/widget/image.c");
new_object("src/widget/label.c");
new_object("src/widget/listbox.c");
new_object("src/widget/menu.c");
new_object("src/widget/numberentry.c");
new_object("src/widget/progressbar.c");
new_object("src/widget/radiobox.c");
new_object("src/widget/scrollbar.c");
new_object("src/widget/separator.c");
new_object("src/widget/submenu.c");
new_object("src/widget/treeview.c");
new_object("src/widget/viewport.c");
new_object("src/widget/window.c");

if (param_get("opengl")) {
    new_object("src/widget/opengl.c");
}
if (param_get("vulkan")) {
    new_object("src/widget/vulkan.c");
}

new_object("src/dialog/*.c");

new_object("src/abstract/*.c");

new_object("external/*.c");

new_example("examples/basic/example");
new_example("examples/basic/rotate");
new_example("examples/basic/image");
new_example("examples/basic/scrollbar");
new_example("examples/basic/checkbox");
new_example("examples/basic/radiobox");
new_example("examples/basic/messagebox");
new_example("examples/basic/viewport");
new_example("examples/basic/listbox");
new_example("examples/basic/progressbar");
new_example("examples/basic/colorpicker");
new_example("examples/basic/combobox");
new_example("examples/basic/treeview");

if (param_get("opengl")) {
    new_example("examples/gldemos/boing",    $gl_libs);
    new_example("examples/gldemos/clock",    $gl_libs);
    new_example("examples/gldemos/cube",     $gl_libs);
    new_example("examples/gldemos/gears",    $gl_libs);
    new_example("examples/gldemos/triangle", $gl_libs);
    new_example("examples/gldemos/tripaint", $gl_libs);
}

if (param_get("vulkan")) {
    new_example("examples/vkdemos/vulkan");
}

1;
