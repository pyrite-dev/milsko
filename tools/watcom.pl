#!/usr/bin/env perl
# $Id$

my $target = "nt";
my @cmd    = ();
push(@cmd, "-I$ENV{WATCOM}/h");
push(@cmd, "-I$ENV{WATCOM}/h/nt");
push(@cmd, "-DSTBI_NO_SIMD");
foreach my $f (@ARGV) {
    if ($f eq "-static-libgcc") {
        next;
    }
    elsif ($f =~ /^-Wl,--out-implib,(.+)$/) {
        push(@cmd, "-Wl,option implib=$1");
        next;
    }
    elsif ($f =~ /^-W.+$/) {
        next;
    }
    elsif ($f =~ /^-L(.+)$/) {
        push(@cmd, "-Wl,libpath $1");
        next;
    }
    elsif ($f eq "-lMw") {
        push(@cmd, "-lMw");
        next;
    }
    elsif ($f =~ /^-l(.+)$/) {
        push(@cmd, "$1.lib");
        next;
    }
    elsif ($f =~ /^(?:oo)?src\//) {
        $target = "nt_dll";
    }
    push(@cmd, $f);
}

unshift(@cmd, "-b$target");

print("Running: owcc " . join(" ", @cmd) . "\n");
exec("owcc", @cmd);
