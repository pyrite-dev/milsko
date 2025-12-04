#!/usr/bin/env perl
open(IN,  "<", "resource/rgb.txt");
open(OUT, ">", "src/color.c");

print(OUT "#include <Mw/Milsko.h>\n");
print(OUT "\n");
print(OUT "MwLLColor MwParseColorName(MwWidget handle, const char* color){\n");
while (my $l = <IN>) {
    $l =~ s/\r?\n$//;
    if ($l =~ /^[ \t]*([0-9]+)[ \t]+([0-9]+)[ \t]+([0-9]+)[ \t]+(.+)$/) {
        print(OUT
"	if(strcmp(color, \"$4\") == 0) return MwLLAllocColor(handle->lowlevel, $1, $2, $3);\n"
        );
    }
}
print(OUT "	return MwLLAllocColor(handle->lowlevel, 0, 0, 0);\n");
print(OUT "}\n");

close(OUT);
close(IN);
