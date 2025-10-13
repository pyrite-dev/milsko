#!/usr/bin/env perl
# $Id$

our $WIDTH = 80;

sub l {
	my $len = 0;
	my $last = 0;
	my @msg = split(/ /, $_[0]);
	foreach my $m (@msg){
		$len += 1 + length($m);
		$last = 0;
		if($len > $WIDTH){
			print(OUT "\n");
			$len = 0;
			$last = 1;
		}
		print(OUT "$m ");
	}
	if(!$last){
		print(OUT "\n");
	}
}

sub c {
	my ($msg) = @_;
	my $len = length($msg);
	if($_[1]){
		$len = $_[1];
	}
	print(OUT (" " x (($WIDTH - $len) / 2)) . "$msg\n");
}

open(OUT, ">", "README.txt");

l("");
l("Greetings - Welcome to the Milsko GUI Toolkit (Version 1.0)");
l("");
l("  This document contains a brief summary of the contents of this source distributions and building instructions for Milsko GUI Toolkit.");

l("");
c("Requirements");
l("");
l("  Milsko requires the Windows environment with GDI (so anything NT or 9x) or the Unix-like environment with X11 for runtime.");
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
l("TODO: Write here");

l("");
l(" -- Nishi  (nishi\@nishi.boats)");

close(OUT);
