#!/usr/bin/env perl
# $Id$

our $WIDTH = 80;

sub l {
    my $len  = 0;
    my $last = 0;
    my @msg  = split(/ /, $_[0]);
    foreach my $m (@msg) {
        $len += 1 + length($m);
        $last = 0;
        if ($len > $WIDTH) {
            print(OUT "\n");
            $len  = 0;
            $last = 1;
        }
        print(OUT "$m ");
    }
    if (!$last) {
        print(OUT "\n");
    }
}

sub c {
    my ($msg) = @_;
    my $len = length($msg);
    if ($_[1]) {
        $len = $_[1];
    }
    print(OUT (" " x (($WIDTH - $len) / 2)) . "$msg\n");
}

sub h {
    my ($msg) = @_;
    print(OUT "\n");
    print(OUT "$msg\n");
    print(OUT ("-" x length($msg)) . "\n");
    print(OUT "\n");
}

open(OUT, ">", "README.txt");

l("");
l("Greetings - Welcome to the Milsko GUI Toolkit (Version 1.0)");
l("");
l(
"  This document contains a brief summary of the contents of this source distributions and building instructions for Milsko GUI Toolkit."
);

l("");
c("Requirements");
l("");
l(
"  Milsko requires the Windows environment with GDI (so anything NT or 9x) or the Unix-like environment with X11 for runtime."
);
l("");
l("  To build Milsko for Windows, you must have one of following compilers:");
l("  * Visual C++ 6.0 or newer");
l("  * Borland C++ 5.5 or newer");
l("  * Open Watcom 2.0 or newer");
l("  * MinGW-w64");
l("");
l("  and for Unix-like:");
l("  * GNU C Compiler");
l("  * Clang");
l("");

l("");
c("Contents");
l("");
c("  At the top level of this hierarchy there are seven directories:", 60);
l("");
c("  src -      Contains the source for Milsko library", 60);
l("");
c("  oosrc -    Contains the source for Milsko OO binding", 60);
l("");
c("  include -  Contains the headers for Milsko", 60);
l("");
c("  doc -      Contains the documentation for Milsko", 60);
l("");
c("  external - Contains the external dependency that Milsko uses", 60);
l("");
c("  resource - Contains the resources used by maintainers", 60);
l("");
c("  tools -    Contains the tools used by maintainers", 60);
l("");

l("");
c("Building Milsko");
l("");
l("  Building Milsko depends on the platform you use, and the compiler you use."
);
h("A. Visual C++");
l("1) Run `nmake -f NTMakefile'.");
h("B. Borland C++");
l("1) Run `make -f BorMakefile'.");
h("C. Open Watcom");
l("1) Run `wmake -f WatMakefile'.");
h("D. MinGW-w64/GCC/Clang");
l("1) Determine if you need Vulkan and/or OpenGL.");
l("");
l("2) Run `make'.");
l("");

l("");
c("Makefile settings");
l("");
c("This section exists for people who chose D.", 60);
l("");
c("TARGET        - Target environment, chosen by Makefile in", 60);
c("                default settings.",                         60);
l("");
c("GCC           - Prefix for GCC toolchains. This does not", 60);
c("                matter if you set CC/CXX manually.",       60);
l("");
c("CC            - C compiler.", 60);
l("");
c("CXX           - C++ compiler.", 60);
l("");
c("DEBUG         - Debug build or not, 0 in default settings.", 60);
l("");
c("VULKAN        - Build Vulkan widget or not.", 60);
l("");
c("OPENGL        - Build OpenGL widget or not.", 60);
l("");
c("USE_STB_IMAGE - Use stb_image or not.", 60);
l("");
c("PREFIX        - Installation destination directory.", 60);
l("");

l("");
c("Makefile targets");
l("");
c("This section exists for people who chose D.", 60);
l("");
c("lib      - Builds Milsko library", 60);
l("");
c("oolib    - Builds OO binding", 60);
l("");
c("examples - Builds examples", 60);
l("");
c("all      - Builds lib and examples", 60);
l("");
c("install  - Installs Milsko to PREFIX", 60);
l("");

l(" -- Nishi  (nishi\@nishi.boats)");

close(OUT);
