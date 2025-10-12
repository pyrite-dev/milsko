#!/usr/bin/env perl
# $Id$

my @cfiles   = ();
my @cxxfiles = ();

sub scan {
    opendir(DIR, $_[0]);
    my @files = readdir(DIR);
    foreach my $f (@files) {
        if ($f =~ /^vulkan\.cc?$/) {
            next;
        }
        if ($f =~ /\.c$/) {
            push(@cfiles, $_[0] . "/" . $f);
        }
        elsif ($f =~ /\.cc$/) {
            push(@cxxfiles, $_[0] . "/" . $f);
        }
    }
    closedir(DIR);
}

sub cobjs {
    my $r = "";
    foreach my $f (@cfiles) {
        my $b = $f;
        $b =~ s/\.cc?$/.obj/;
        $b =~ s/\//$_[0]/g;

        $r = $r . " " . $_[1] . $b;
    }
    return $r;
}

sub cxxobjs {
    my $r = "";
    foreach my $f (@cxxfiles) {
        my $b = $f;
        $b =~ s/\.cc?$/.obj/;
        $b =~ s/\//$_[0]/g;

        $r = $r . " " . $_[1] . $b;
    }
    return $r;
}

sub generate {
    my ($output, $type) = @_;

    my $cc   = "";
    my $cxx  = "";
    my $link = "";

    my $suffix       = 1;
    my $symbolic     = "";
    my $dir          = "\\";
    my $del          = "del /f /q";
    my $out          = "";
    my $dllout       = "";
    my $def          = "";
    my $inc          = "";
    my $dll          = "";
    my $prefobj      = "";
    my $needlibs     = "";
    my $lib          = "";
    my $c_dllout     = "";
    my $cxx_dllout   = "";
    my $c_dllafter   = "";
    my $cxx_dllafter = "";

    if ($type eq "Borland") {
        $cc     = "bcc32 -c";
        $cxx    = "bcc32 -c -P";
        $link   = "bcc32";
        $out    = "-o";
        $dllout = "-e";
        $def    = "-D";
        $inc    = "-I";
        $dll    = "-tWD";
        $lib    = "-l";

        $c_dllafter   = "implib src${dir}Mw.lib src${dir}Mw.dll";
        $cxx_dllafter = "implib oosrc${dir}MwOO.lib oosrc${dir}MwOO.dll";
    }
    elsif ($type eq "MSVC") {
        $cc     = "cl /TC /c /nologo";
        $cxx    = "cl /TP /c /nologo";
        $link   = "link /nologo";
        $out    = "/Fo";
        $dllout = "/OUT:";
        $def    = "/D";
        $inc    = "/I";
        $dll    = "/DLL";
    }
    elsif ($type eq "Watcom") {
        $cc     = "wcc386 -bt=nt -q";
        $cxx    = "wpp386 -bt=nt -q";
        $link   = "wlink";
        $out    = "-fo=";
        $dllout = "name ";
        $def    = "-d";
        $inc    = "-i=";
        $dll    = "system nt_dll";
        $lib    = "library ";

        $suffix   = 0;                  # is watcom make broken with suffix rule
        $symbolic = ".SYMBOLIC";
        $dir      = "/";
        $del      = "%erase";
        $prefobj  = "file ";
        $needlibs = "${lib}clib3r.lib";
        $c_dllout   = "option implib=src${dir}Mw.lib";
        $cxx_dllout = "option implib=oosrc${dir}MwOO.lib";
    }

    open(OUT, ">", $output);
    print(OUT "# \$Id\$\n");
    print(OUT "CC = $cc\n");
    print(OUT "CXX = $cxx\n");
    print(OUT "LD = $link\n");
    print(OUT "\n");
    print(OUT
"CFLAGS = ${inc}include ${def}_MILSKO ${def}USE_GDI ${def}USE_STB_IMAGE ${def}STBI_NO_SIMD\n"
    );
    print(OUT "CXXFLAGS = ${inc}include\n");
    print(OUT "LDFLAGS = $dll");
    print(OUT "\n");

    if ($suffix) {
        print(OUT ".SUFFIXES: .c .cc .obj\n");
    }
    print(OUT "all: src${dir}Mw.dll oosrc${dir}MwOO.dll\n");
    print(OUT "clean: $symbolic\n");
    foreach my $f (@cfiles) {
        my $b = $f;
        $b =~ s/\.cc?$/.obj/;
        $b =~ s/\//$dir/g;
        print(OUT "	$del $b\n");
    }
    foreach my $f (@cxxfiles) {
        my $b = $f;
        $b =~ s/\.cc?$/.obj/;
        $b =~ s/\//$dir/g;
        print(OUT "	$del $b\n");
    }
    print(OUT "	$del src${dir}Mw.dll\n");
    print(OUT "	$del oosrc${dir}MwOO.dll\n");
    print(OUT "\n");
    print(OUT "src${dir}Mw.dll: " . cobjs($dir) . "\n");
    print(  OUT "	\$(LD) \$(LDFLAGS) $c_dllout $dllout\$@ "
          . cobjs($dir, $prefobj)
          . " $needlibs ${lib}gdi32.lib ${lib}user32.lib\n");
    print(OUT "	$c_dllafter\n");
    print(OUT "\n");
    print(OUT "oosrc${dir}MwOO.dll: " . cxxobjs($dir) . "\n");
    print(  OUT "	\$(LD) \$(LDFLAGS) $cxx_dllout $dllout\$@ "
          . cxxobjs($dir, $prefobj)
          . " $needlibs ${lib}src${dir}Mw.lib\n");
    print(OUT "	$cxx_dllafter\n");
    print(OUT "\n");

    if ($suffix) {
        print(OUT ".c.obj:\n");
        print(OUT "	\$(CC) \$(CFLAGS) ${out}\$@ \$<\n");
        print(OUT ".cc.obj:\n");
        print(OUT "	\$(CXX) \$(CXXFLAGS) ${out}\$@ \$<\n");
    }
    else {
        print(OUT "\n");
        foreach my $f (@cfiles) {
            my $o = $f;
            $o =~ s/\.cc?$/.obj/;
            print(OUT "$o: $f\n");
            print(OUT "	\$(CC) \$(CFLAGS) ${out}\$@ \$<\n");
        }
        foreach my $f (@cxxfiles) {
            my $o = $f;
            $o =~ s/\.cc?$/.obj/;
            print(OUT "$o: $f\n");
            print(OUT "	\$(CXX) \$(CXXFLAGS) ${out}\$@ \$<\n");
        }
    }
    close(OUT);
}

scan("src");
scan("src/external");
scan("src/icon");
scan("src/cursor");
scan("src/widget");
push(@cfiles, "src/backend/gdi.c");

scan("oosrc");
scan("oosrc/widget");

generate("BorMakefile", "Borland");
generate("NTMakefile",  "MSVC");
generate("WatMakefile", "Watcom");
