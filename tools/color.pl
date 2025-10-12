#!/usr/bin/env perl
# $Id$

open(IN,  "<", "resource/rgb.txt");
open(OUT, ">", "src/color.c");

print(OUT "/* \$Id\$ */\n");
print(OUT "#include <Mw/Milsko.h>\n");
print(OUT "\n");
print(OUT "MwLLColor MwParseColorName(MwWidget handle, const char* color){\n");
print(OUT "	int r = 0, g = 0, b = 0;\n");
print(OUT "	\n");
while (my $l = <IN>) {
    $l =~ s/\r?\n$//;
    if ($l =~ /^[ \t]*([0-9]+)[ \t]+([0-9]+)[ \t]+([0-9]+)[ \t]+(.+)$/) {
        print(OUT "	if(strcmp(color, \"$4\") == 0){\n");
        print(OUT "		r = $1;\n");
        print(OUT "		g = $2;\n");
        print(OUT "		b = $3;\n");
        print(OUT "	}\n");
    }
}
print(OUT "	return MwLLAllocColor(handle->lowlevel, r, g, b);\n");
print(OUT "}\n");

close(OUT);
close(IN);
