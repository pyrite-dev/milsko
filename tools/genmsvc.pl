#!/usr/bin/env perl
# $Id$

sub scan {
    opendir(DIR, $_[0]);
    my @files = readdir(DIR);
    foreach my $f (@files) {
        if (!($f =~ /^vulkan\.cc?$/) && ($f =~ /\.cc?$/)) {
            $f =~ s/\.cc?$/.obj/g;
            my $p = $_[0] . "/" . $f;
            $p =~ s/\//\\/g;
            print(OUT " $p");
        }
    }
    closedir(DIR);
}

open(OUT, ">", "NTMakefile");
print(OUT "# \$Id\$\n");
print(OUT "CC = cl\n");
print(OUT "LD = link\n");
print(OUT "\n");
print(OUT "OBJS =");
scan("src");
scan("src/widget");
scan("src/external");
scan("src/cursor");
print(OUT " src\\backend\\gdi.obj\n");
print(OUT "OOOBJS =");
scan("oosrc");
scan("oosrc/widget");
print(OUT "\n");
print(OUT
"CFLAGS = /nologo /DSTBI_NO_SIMD /D_MILSKO /DUSE_GDI /DUSE_STB_IMAGE /Iinclude\n"
);
print(OUT "CXXFLAGS = /nologo /Iinclude\n");
print(OUT "LDFLAGS = /nologo /DLL /LIBPATH:src /LIBPATH:oosrc\n");
print(OUT "\n");
print(OUT "all: src\\Mw.dll oosrc\\MwOO.dll\n");
print(OUT "\n");
print(OUT ".SUFFIXES: .cc .c .obj\n");
print(OUT "\n");
print(OUT ".c.obj:\n");
print(OUT "\t\$(CC) \$(CFLAGS) /TC /c /Fo\$@ \$<\n");
print(OUT "\n");
print(OUT ".cc.obj:\n");
print(OUT "\t\$(CC) \$(CXXFLAGS) /TP /c /Fo\$@ \$<\n");
print(OUT "\n");
print(OUT "src\\Mw.dll: \$(OBJS)\n");
print(OUT
      "\t\$(LD) \$(LDFLAGS) /OUT:src\\Mw.dll \$(OBJS) gdi32.lib user32.lib\n");
print(OUT "oosrc\\MwOO.dll: \$(OOOBJS)\n");
print(OUT "\t\$(LD) \$(LDFLAGS) /OUT:oosrc\\MwOO.dll \$(OOOBJS) Mw.lib\n");
print(OUT "\n");
print(OUT "clean:\n");
print(OUT "\tdel /q \$(OBJS)\n");
print(OUT "\tdel /q \$(OOOBJS)\n");
print(OUT
"\tdel /q src\\Mw.dll src\\Mw.lib src\\Mw.exp oosrc\\MwOO.dll oosrc\\MwOO.lib oosrc\\MwOO.exp\n"
);
close(OUT);
