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
            push(@sources, $_[0] . "/" . $f);
            $f =~ s/\.cc?$/.obj/g;
            my $p = $_[0] . "/" . $f;
            print(OUT " $p");
        }
    }
    closedir(DIR);
}

open(OUT, ">", "WatMakefile");
print(OUT "# \$Id\$\n");
print(OUT "WATCOM = /usr/watcom\n");
print(OUT "CC = wcc386\n");
print(OUT "CXX = wpp386\n");
print(OUT
"CFLAGS = -q -d_MILSKO -dUSE_GDI -dUSE_STB_IMAGE -dSTBI_NO_SIMD -i=\$(WATCOM)/h/nt -i=include\n"
);
print(OUT "CXXFLAGS = -q -i=include -i=\$(WATCOM)/h/nt\n");
print(OUT "\n");
print(OUT "OBJS =");
scan("src");
scan("src/widget");
scan("src/external");
scan("src/cursor");
scan("src/icon");
push(@sources, "src/backend/gdi.c");
@c_sources = @sources;
@sources   = ();
print(OUT "\n");
print(OUT "OOOBJS =");
scan("oosrc");
scan("oosrc/widget");
@cxx_sources = @sources;
@sources     = ();
print(OUT "\n");
print(OUT "\n");
print(OUT "all: src/Mw.dll oosrc/MwOO.dll\n");
print(OUT "\n");
print(OUT "clean: .SYMBOLIC\n");

foreach my $l (@c_sources) {
    my $obj = $l;
    $obj =~ s/\.cc?/.obj/g;
    print(OUT "	%erase $obj\n");
}
foreach my $l (@cxx_sources) {
    my $obj = $l;
    $obj =~ s/\.cc?/.obj/g;
    print(OUT "	%erase $obj\n");
}
print(OUT "	%erase src/Mw.dll\n");
print(OUT "	%erase src/Mw.lib\n");
print(OUT "	%erase oosrc/MwOO.dll\n");
print(OUT "	%erase oosrc/MwOO.lib\n");
print(OUT "\n");
print(OUT "oosrc/MwOO.dll: \$(OOOBJS) src/Mw.dll\n");
print(OUT
"	%write MwOO.lnk system nt_dll initinstance terminstance name oosrc/MwOO.dll\n"
);
print(OUT "	%write MwOO.lnk option implib=oosrc/MwOO.lib\n");
print(OUT "	%write MwOO.lnk library src/Mw.lib\n");
print(OUT "	%write MwOO.lnk library clib3r.lib\n");

foreach my $l (@cxx_sources) {
    my $obj = $l;
    $obj =~ s/\.cc?/.obj/g;
    print(OUT "	%write MwOO.lnk file $obj\n");
}
print(OUT "	wlink \@MwOO.lnk\n");
print(OUT "	%erase MwOO.lnk");
print(OUT "\n");
print(OUT "src/Mw.dll: \$(OBJS)\n");
print(OUT
      "	%write Mw.lnk system nt_dll initinstance terminstance name src/Mw.dll\n"
);
print(OUT "	%write Mw.lnk option implib=src/Mw.lib\n");
print(OUT "	%write Mw.lnk library clib3r.lib\n");

foreach my $l (@c_sources) {
    my $obj = $l;
    $obj =~ s/\.cc?/.obj/g;
    print(OUT "	%write Mw.lnk file $obj\n");
}
print(OUT "	wlink \@Mw.lnk\n");
print(OUT "	%erase Mw.lnk");
print(OUT "\n");
foreach my $l (@c_sources) {
    my $obj = $l;
    $obj =~ s/\.cc?/.obj/g;
    print(OUT "$obj: $l\n");
    print(OUT "	\$(CC) \$(CFLAGS) -bt=nt -fo=\$@ $l\n");
}
foreach my $l (@cxx_sources) {
    my $obj = $l;
    $obj =~ s/\.cc?/.obj/g;
    print(OUT "$obj: $l\n");
    print(OUT "	\$(CXX) \$(CXXFLAGS) -bt=nt -fo=\$@ $l\n");
}
close(OUT);
