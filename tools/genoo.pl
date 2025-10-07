#!/usr/bin/env perl
# $Id$

use Config::Tiny;

my $conf = Config::Tiny->read("resource/widget.ini");

my $objs = "";

my $decl  = "";
my @calls = ();
open(IN, "<", "include/Mw/StringDefs.h");
while (my $hl = <IN>) {
    $hl =~ s/\r?\n$//;
    if ($hl =~ /^#define[ \t]+([^ ]+)[ \t]+"C(.+)"$/) {
        my $name = "On" . uc(substr($2, 0, 1)) . substr($2, 1);
        my @c    = ($1, $name);

        $decl = $decl . "	virtual void " . $name . "(void*){};\n";

        push(@calls, [@c]);
    }
}
close(HIN);

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
    print(OUT "namespace MwOO {\n");
    print(OUT "	class ${name} : public MwOO::Base {\n");
    print(OUT "	public:\n");
    print(OUT
"		${name}(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h);\n"
    );
    print(OUT "	};\n");
    print(OUT "}\n");
    print(OUT "\n");
    print(OUT "#endif\n");
    close(OUT);

    open(OUT, ">", "oosrc/widget/" . lc($name) . ".cc");
    print(OUT "/* \$Id\$ */\n");
    print(OUT "#include <MwOO/Widget/$name.h>\n");
    print(OUT "#include <Mw/Widget/$name.h>\n");
    print(OUT "\n");
    print(OUT
"MwOO::${name}::${name}(const char* widget_name, MwOO::Base* parent, int x, int y, int w, int h) : MwOO::Base(Mw${name}Class, widget_name, parent, x, y, w, h){\n"
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

my $base_h = "";
my $skip   = 0;

open(IN, "<", "include/MwOO/Base.h");
while (my $l = <IN>) {
    $l =~ s/\r?\n$//;
    if ($l =~ /BEGIN AUTOGENERATE/) {
        $base_h = $base_h . "/* BEGIN AUTOGENERATE */\n";
        $skip   = 1;
        $base_h = $base_h . $decl . "\n";
    }
    elsif ($l =~ /END AUTOGENERATE/) {
        $base_h = $base_h . "/* END AUTOGENERATE */\n";
        $skip   = 0;
    }
    elsif (!$skip) {
        $base_h = $base_h . $l . "\n";
    }
}
close(IN);

open(OUT, ">", "include/MwOO/Base.h");
print(OUT "$base_h");
close(OUT);

my $base_c = "";
my $skip   = 0;

open(IN, "<", "oosrc/base.cc");
while (my $l = <IN>) {
    $l =~ s/\r?\n$//;
    if ($l =~ /BEGIN AUTOGENERATE/) {
        $base_c = $base_c . "/* BEGIN AUTOGENERATE */\n";
        $skip   = 1;

        foreach my $call (@calls) {
            my @c = @{$call};
            $base_c =
                $base_c
              . "static void __"
              . $c[1]
              . "(MwWidget widget, void* user, void* call){\n";
            $base_c = $base_c . "	MwOO::Base* c = (MwOO::Base*)user;\n";
            $base_c = $base_c . "	c->" . $c[1] . "(call);\n";
            $base_c = $base_c . "}\n";
        }
        $base_c = $base_c . "\n";

        $base_c = $base_c . "void MwOO::Base::SetHandler(void){\n";
        foreach my $call (@calls) {
            my @c = @{$call};
            $base_c =
                $base_c
              . "	MwAddUserHandler(this->widget, "
              . $c[0] . ", __"
              . $c[1]
              . ", this);\n";
        }
        $base_c = $base_c . "}\n";
    }
    elsif ($l =~ /END AUTOGENERATE/) {
        $base_c = $base_c . "/* END AUTOGENERATE */\n";
        $skip   = 0;
    }
    elsif (!$skip) {
        $base_c = $base_c . $l . "\n";
    }
}
close(IN);

open(OUT, ">", "oosrc/base.cc");
print(OUT "$base_c");
close(OUT);
