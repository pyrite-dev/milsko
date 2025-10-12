#!/usr/bin/env perl
# $Id$

our @sources     = ();
our @c_sources   = ();
our @cxx_sources = ();

sub scan {
    opendir(DIR, $_[0]);
    my @files = readdir(DIR);
    foreach my $f (@files) {
        if (!($f =~ /^vulkan\.cc?$/) && ($f =~ /\.cc?$/)) {
            my $p = $_[0] . "/" . $f;
            $p =~ s/\//\\/g;
            push(@sources, $p);
            $p =~ s/\.cc?$/.obj/g;
            print(OUT " $p");
        }
    }
    closedir(DIR);
}

open(OUT, ">", "BorMakefile");
print(OUT "# \$Id\$\n");
print(OUT "CC = bcc32\n");
print(OUT "CXX = bcc32\n");
print(OUT "CFLAGS = -D_MILSKO -DUSE_GDI -DUSE_STB_IMAGE -DSTBI_NO_SIMD -Iinclude\n");
print(OUT "CXXFLAGS = -Iinclude\n");
print(OUT "\n");
print(OUT "OBJS =");
scan("src");
scan("src/widget");
scan("src/external");
scan("src/cursor");
scan("src/icon");
push(@sources, "src\\backend\\gdi.c");
@c_sources = @sources;
@sources   = ();
print(OUT " src\\backend\\gdi.obj\n");
print(OUT "OOOBJS =");
scan("oosrc");
scan("oosrc/widget");
@cxx_sources = @sources;
@sources     = ();
print(OUT "\n");
print(OUT "\n");
print(OUT "all: src\\Mw.dll oosrc\\MwOO.dll\n");
print(OUT "\n");
print(OUT ".SUFFIXES: .c .cc .obj\n");
print(OUT "\n");
print(OUT "clean:\n");

foreach my $l (@c_sources) {
    my $obj = $l;
    $obj =~ s/\.cc?/.obj/g;
    print(OUT "	del /q /f $obj\n");
}
foreach my $l (@cxx_sources) {
    my $obj = $l;
    $obj =~ s/\.cc?/.obj/g;
    print(OUT "	del /q /f $obj\n");
}
print(OUT "	del /q /f src\\Mw.dll\n");
print(OUT "	del /q /f src\\Mw.lib\n");
print(OUT "	del /q /f oosrc\\MwOO.dll\n");
print(OUT "	del /q /f oosrc\\MwOO.lib\n");
print(OUT "\n");
print(OUT "oosrc\\MwOO.dll: \$(OOOBJS) src\\Mw.dll\n");
print(OUT "	\$(CC) -tWD -eoosrc\\MwOO.dll \$(OOOBJS) src\\Mw.lib\n");
print(OUT "	implib oosrc\\MwOO.lib oosrc\\MwOO.dll");
print(OUT "\n");
print(OUT "src\\Mw.dll: \$(OBJS)\n");
print(OUT "	\$(CC) -tWD -esrc\\Mw.dll \$(OBJS)\n");
print(OUT "	implib src\\Mw.lib src\\Mw.dll");
print(OUT "\n");
print(OUT ".c.obj:\n");
print(OUT "	\$(CC) \$(CFLAGS) -c -o\$@ \$< \n");
print(OUT ".cc.obj:\n");
print(OUT "	\$(CXX) -P \$(CXXFLAGS) -c -o\$@ \$< \n");
close(OUT);
