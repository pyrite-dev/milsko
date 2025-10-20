#!/usr/bin/env perl
# $Id$

my $objs = "";

my $decl  = "";
my @calls = ();
my %ints  = ();
my %texts = ();
my %voids = ();
open(IN, "<", "include/Mw/StringDefs.h");
while (my $hl = <IN>) {
    $hl =~ s/\r?\n$//;
    if ($hl =~ /^#define[ \t]+([^ ]+)[ \t]+"C(.+)"$/) {
        my $name = "On" . uc(substr($2, 0, 1)) . substr($2, 1);
        my @c    = ($1, $name);

        $decl = $decl . "	virtual void " . $name . "(void*){};\n";

        push(@calls, [@c]);
    }
    elsif ($hl =~ /^#define[ \t]+([^ ]+)[ \t]+"I(.+)"$/) {
        $ints{$1} = uc(substr($2, 0, 1)) . substr($2, 1);
    }
    elsif ($hl =~ /^#define[ \t]+([^ ]+)[ \t]+"S(.+)"$/) {
        $texts{$1} = uc(substr($2, 0, 1)) . substr($2, 1);
    }
    elsif ($hl =~ /^#define[ \t]+([^ ]+)[ \t]+"V(.+)"$/) {
        $voids{$1} = uc(substr($2, 0, 1)) . substr($2, 1);
    }
}
close(IN);

opendir(DIR, "include/Mw/Widget");
my @files = readdir(DIR);
foreach my $f (@files) {
    if ($f eq '.' || $f eq '..') {
        next;
    }

    $f =~ /^(.+)\.h$/;

    my $name     = $1;
    my @props    = ();
    my @methods  = ();
    my @omethods = ();
    my @comments = ();
    my @names    = ();
    my @args     = ();

    open(IN, "include/Mw/Widget/$f");
    while (my $l = <IN>) {
        $l =~ s/\r?\n$//g;

        if ($l =~ /%prop[ \t]+(.+)$/) {
            @props = split(/[ \t]+/, $1);
        }
        elsif ($l =~
/^(?:MWDECL|MwInline)[ \t]+(.+)[ \t]+Mw${name}([^ \t]+)[ \t]*\([^,\)]+(?:,(.*))?\)(?:;|[ \t]+\{)(?: \/\*(.+))?$/
          )
        {
            my $arg = $3;
            my $ret = $1;
            my $nam = $2;
            my $com = $4;

            $arg =~ s/^[ \t]+//;
            $arg =~ s/[ \t]+$//;

            if (!$arg) {
                $arg = "void";
            }

            push(@methods,  "$ret $nam($arg)");
            push(@omethods, "$ret MwOO::${name}::$nam($arg)");
            push(@names,    $nam);
            push(@comments, $com);

            my @al = split(/[ \t]*,[ \t]*/, $arg);
            my $i  = 0;
            foreach my $dummy (@al) {
                $al[$i] =~ s/^.+[ \t]+([^ \t]+)$/\1/g;
                $i++;
            }

            push(@args, join(", ", @al));
        }
    }
    close(IN);

    push(@props, "MwNbackground");
    push(@props, "MwNforeground");

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

    foreach my $m (@methods) {
        if ($m =~ /^MwWidget[ \t]+/) {
            my $l = $m;
            $l =~ s/^MwWidget([ \t]+)/MwOO::Base\1/g;
            print(OUT "		$l;\n");
        }
        else {
            print(OUT "		$m;\n");
        }
    }

    foreach my $prop (@props) {
        my $type = "";
        my %hash = ();
        if (defined($ints{$prop})) {
            $type = "int";
            %hash = %ints;
        }
        elsif (defined($texts{$prop})) {
            $type = "const char*";
            %hash = %texts;
        }
        elsif (defined($voids{$prop})) {
            $type = "void*";
            %hash = %voids;
        }
        if (length($type) > 0) {
            print(OUT "		void Set" . $hash{$prop} . "($type value);\n");
            print(OUT "		$type Get" . $hash{$prop} . "(void);\n");
        }
    }
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
    print(OUT "\n");

    my $i = 0;
    foreach my $m (@omethods) {
        my $end    = "";
        my $suffix = "";
        if ($m =~ /^MwWidget[ \t]+/) {
            my $l = $m;
            $l =~ s/^MwWidget([ \t]+)/MwOO::Base\1/;
            print(OUT "$l\{\n");
        }
        else {
            print(OUT "$m\{\n");
        }
        if ($args[$i] =~ /\.\.\./) {
            print(OUT "	va_list va;\n");
        }
        if (!($m =~ /^void[ \t]+/)) {
            my $rettype = $m;
            $rettype =~ s/^MwWidget([ \t]+)/MwOO::Base\1/;
            $rettype =~ s/ MwOO::.+\)$//;
            print(OUT "	$rettype ret;\n");
        }
        if ($args[$i] =~ /\.\.\./) {
            my $p = $comments[$i];
            $p =~ /VA_HINT:([^ ]+)/;
            print(OUT "	va_start(va, $1);\n");
            $suffix = "Va";
        }
        if ($m =~ /^void[ \t]+/) {
            print(OUT "	Mw${name}$suffix" . $names[$i]);
        }
        elsif ($m =~ /^MwWidget[ \t]+/) {
            print(OUT "	ret = MwOO::Base(Mw${name}$suffix" . $names[$i]);
            $end = ")";
        }
        else {
            print(OUT "	ret = Mw${name}$suffix" . $names[$i]);
        }
        if ($args[$i] eq "void") {
            print(OUT "(this->widget)$end;\n");
        }
        else {
            my $p = $args[$i];
            $p =~ s/\.\.\./va/;
            print(OUT "(this->widget, " . $p . ")$end;\n");
        }
        if ($args[$i] =~ /\.\.\./) {
            print(OUT "	va_end(va);\n");
        }
        if (!($m =~ /^void[ \t]+/)) {
            print(OUT "	return ret;\n");
        }
        print(OUT "}\n\n");
        $i++;
    }
    foreach my $prop (@props) {
        my $type     = "";
        my $typename = "";
        my %hash     = ();
        if (defined($ints{$prop})) {
            $type     = "int";
            $typename = "Integer";
            %hash     = %ints;
        }
        elsif (defined($texts{$prop})) {
            $type     = "const char*";
            $typename = "Text";
            %hash     = %texts;
        }
        elsif (defined($voids{$prop})) {
            $type     = "void*";
            $typename = "Void";
            %hash     = %voids;
        }
        if (length($type) > 0) {
            print(  OUT "void MwOO::${name}::Set"
                  . $hash{$prop}
                  . "($type value){\n");
            print(OUT "	MwSet$typename(this->widget, $prop, value);\n");
            print(OUT "}\n");
            print(OUT "\n");
            print(OUT "$type MwOO::${name}::Get" . $hash{$prop} . "(void){\n");
            print(OUT "	return MwGet$typename(this->widget, $prop);\n");
            print(OUT "}\n");
            print(OUT "\n");
        }
    }
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
            $base_c = $base_c . "	(void)widget;";
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
