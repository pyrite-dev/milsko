#!/usr/bin/env perl
# $Id$

my $objs = "";

opendir(DIR, "include/Mw/Widget");
my @files = readdir(DIR);
foreach my $f (@files) {
    if ($f eq '.' || $f eq '..') {
        next;
    }

    $f =~ /^(.+)\.h$/;

    my $name = $1;

    open(OUT, ">", "include/MwOO/Widget/$f");
    print(OUT "/* \$Id\$ */\n");
    print(OUT "#ifndef __MWOO_WIDGET_" . uc($name) . "_H__\n");
    print(OUT "#define __MWOO_WIDGET_" . uc($name) . "_H__\n");
    print(OUT "\n");
    print(OUT "#include <MwOO/Base.h>\n");
    print(OUT "\n");
    print(OUT "class MwOO${name}Widget : public MwOOWidget {\n");
    print(OUT "public:\n");
    print(OUT
"	MwOO${name}Widget(const char* name, MwOOWidget* parent, int x, int y, int w, int h);\n"
    );
    print(OUT "};\n");
    print(OUT "\n");
    print(OUT "#endif\n");
    close(OUT);

    open(OUT, ">", "oosrc/widget/" . lc($name) . ".cc");
    print(OUT "/* \$Id\$ */\n");
    print(OUT "#include <MwOO/Widget/$name.h>\n");
    print(OUT "#include <Mw/Widget/$name.h>\n");
    print(OUT "\n");
    print(OUT
"MwOO${name}Widget::MwOO${name}Widget(const char* name, MwOOWidget* parent, int x, int y, int w, int h) : MwOOWidget(Mw${name}Class, name, parent, x, y, w, h){\n"
    );
    print(OUT "}\n");
    close(OUT);

    if (!($name eq 'Vulkan' || $name eq 'OpenGL')) {
        $objs = $objs . " oosrc/widget/" . lc($name) . ".o";
    }
}
closedir(DIR);

open(OUT, ">", "oosrc/deps.mk");
print(OUT "# \$Id\$\n");
print(OUT "OOL_OBJS +=$objs\n");
close(OUT);
