#!/usr/bin/env perl
# $Id$

open(IN, "<", $ARGV[0]);
while (my $l = <IN>) {
    $l =~ s/\r?\n$//g;
    $l =~ s/%([^ ]+)/@\1/g;
    if (!($l =~ /\@prop /)) {
        print("$l\n");
    }
}
close(IN);
