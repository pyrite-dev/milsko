#!/usr/bin/env perl
open(IN,  "<", "resource/rgb.txt");
open(OUT, ">", "src/color.c");

print(OUT "#include <Mw/Milsko.h>\n");
print(OUT "\n");
print(OUT "MwLLColor MwParseColorName(MwWidget handle, const char* color){\n");
print(OUT "	MwRGB rgb;\n");
print(OUT "	MwParseColorNameNoAllocate(color, &rgb);\n");
print(OUT
"	return MwLLAllocColor(handle->lowlevel, rgb.red, rgb.green, rgb.blue);\n"
);
print(OUT "}\n");
print(OUT "\n");
print(OUT "void MwParseColorNameNoAllocate(const char* color, MwRGB* rgb){\n");

while (my $l = <IN>) {
    $l =~ s/\r?\n$//;
    if ($l =~ /^[ \t]*([0-9]+)[ \t]+([0-9]+)[ \t]+([0-9]+)[ \t]+(.+)$/) {
        print(OUT "	if(strcmp(color, \"$4\") == 0){\n");
        print(OUT "		rgb->red = $1;\n");
        print(OUT "		rgb->green = $2;\n");
        print(OUT "		rgb->blue = $3;\n");
        print(OUT "		return;\n");
        print(OUT "	}\n");
    }
}
print(OUT "	rgb->red = 0;\n");
print(OUT "	rgb->green = 0;\n");
print(OUT "	rgb->blue = 0;\n");
print(OUT "}\n");

close(OUT);
close(IN);
