#!/usr/bin/env perl
# $Id$

our $html     = "";
our $title    = "Milsko GUI Toolkit Documentation";
our @pathlist = ("include");
our @notes    = ("warning", "unsure", "note");

our @files          = ();
our %files_sections = ();

sub sentence {
    my $str = $_[0];
    $str =~ s/([^\.])$/\1./g;
    $str =~ s/`(.+?)`/<code>\1<\/code>/g;
    $str =~ s/\*{3}(.+?)\*{3}/<b><i>\1<\/i><\/b>/g;
    $str =~ s/\*{2}(.+?)\*{2}/<b>\1<\/b>/g;
    $str =~ s/\*(.+?)\*/<i>\1<\/i>/g;
    return $str;
}

sub out {
    $html = $html . "$_[0]\n";
}

sub arguments {
    my ($args)  = @_;
    my $out     = "";
    my @arglist = split(/[ \t]*,[ \t]*/, $args);

    $out = $out . " (\n";
    foreach my $str (@arglist) {
        $out = $out . "\t$str";
        if ($str eq $arglist[ 0 + @arglist - 1 ]) {
            $out = $out . "\n";
        }
        else {
            $out = $out . ",\n";
        }
    }
    $out = $out . ")";

    return $out;
}

sub replace_special {
    my $ret = $_[0];

    $ret =~ s/[\r\n]+//g;
    $ret =~ s/[^A-Za-z0-9_]/_/g;

    return $ret;
}

sub highlight {
    my $txt = "";
    my $app = 0;

    open(DOCPL, ">", "/tmp/docpl");
    print(DOCPL $_[0] . "\n");
    close(DOCPL);

    open(DOCIN, "cat /tmp/docpl | enscript -q -whtml --color -Ec -o - |");
    while (my $l = <DOCIN>) {
        $l =~ s/\r?\n$//g;
        if ($l =~ /\<pre\>/i) {
            $app = 1;
        }
        elsif ($l =~ /\<\/pre\>/i) {
            $app = 0;
        }
        elsif ($app) {
            $txt = $txt . "\n" . $l;
        }
    }
    close(DOCIN);

    $txt =~ s/[\r\n]+$//g;
    $txt =~ s/^[\r\n]+//g;

    return $txt;
}

sub scan_dir {
    my ($first, $path) = @_;
    $path =~ s/\/+$//g;

    if (opendir(my $dh, $path)) {
        print(STDERR "Scanning $path\n");
        my @paths = sort(readdir($dh));
        foreach my $p (@paths) {
            if ($p eq '.' || $p eq '..' || $p eq 'MwOO') {
                next;
            }
            scan_dir($first, $path . "/" . $p);
        }
        closedir($dh);
    }
    else {
        my $spaces    = "";
        my $in        = 0;
        my $out       = 0;
        my $file      = 0;
        my $has_file  = 0;
        my %kv        = ();
        my %param     = ();
        my @paramlist = ();
        my $current   = "";

        print(STDERR "$path is a file\n");
        open(IN, "<", "$path");
        while (my $l = <IN>) {
            my $l2 = $l;
            $l =~ s/[\r\n]+$//g;

            $l2 =~ s/[\r\n \t]+//g;
            if (length($l2) == 0) {
                next;
            }

            if ($out) {
                my $para  = "dt";
                my $brief = $kv{brief} or "";

                $brief = sentence($brief);

                $out = 0;
                if (!$has_file) {
                    print(STDERR
                          "Warning: missing comment with %file, ignoring\n");
                    next;
                }
                if (!defined($kv{brief})) {
                    print(STDERR "Warning: comment missing %brief, ignoring\n");
                    next;
                }
                if (!$file && defined($kv{brief})) {
                    my $id   = "";
                    my $sl   = "";
                    my $attr = "";

                    $para = "dd";

                    $l =~ s/[ \t]+/ /g;
                    $l =~ s/[ ]*(?:\\|,[ ]*)$//g;
                    $l =~ s/[ ]*(?:;|\{.*)[ ]*$/;/g;
                    if (!($l =~ /^#/)) {
                        $l =~ s/\(([^\)]+)\)/arguments($1)/ge;
                    }

                    $sl = $l;
                    $sl =~ s/\n//g;
                    $sl =~ s/=.+$//g;
                    $sl =~ s/^.+[ \t]+([^ ]+)[ \t]*\(.+\);?$/\1/g;
                    $sl =~ s/^.+[ \t]+([^ ]+);$/\1/g;

                    $id   = replace_special($current . "__" . $sl);
                    $attr = " id=\"$id\"";

                    if (!defined($files_sections{ replace_special($current) }))
                    {
                        @{ $files_sections{ replace_special($current) } } = ();
                    }
                    push(@{ $files_sections{ replace_special($current) } },
                        $sl);

                    out("<pre$attr>" . highlight($l) . "</pre>");
                }
                out("<dl>");
                out("<$para>");
                out("	$brief");
                out("</$para>");

                foreach my $note (@notes) {
                    if (defined($kv{$note})) {
                        out("<dt>");
                        out(
"	<table border=\"0\"><tr><td><img src=\"$note.gif\" alt=\"$note\"></td><td>"
                              . sentence($kv{$note})
                              . "</td></table>");
                        out("</dt>");
                    }
                }

                foreach my $p (@paramlist) {
                    out("<dt>");
                    out("	Parameter <code>$p</code>");
                    out("</dt>");
                    out("<dd>");
                    out("	" . sentence($param{$p}));
                    out("</dd>");
                }

                if (defined($kv{return})) {
                    out("<dt>");
                    out("	Returns");
                    out("</dt>");
                    out("<dd>");
                    out("	" . sentence($kv{return}));
                    out("</dd>");
                }
                out("</dl>");
                out("<hr>");
            }
            elsif ($l =~ /^([ \t]*)\/\*\!/) {
                $spaces    = $1;
                $in        = 1;
                $file      = 0;
                %kv        = ();
                %param     = ();
                @paramlist = ();
            }
            elsif ($in && ($l =~ /^$spaces \*\//)) {
                $in  = 0;
                $out = 1;
            }
            elsif ($in
                && ($l =~ /^$spaces \*[ \t]+%([^ ]+)(?:[ \t]+(.+))?$/))
            {
                $kv{$1} = $2;
                if ($1 eq "file") {
                    if (!$has_file) {
                        out(    "<h2 align=\"center\" id=\""
                              . replace_special($2)
                              . "\">$2</h2>");
                        push(@files, $2);
                    }

                    $file     = 1;
                    $has_file = 1;
                    $current  = $2;
                }
                elsif ($1 eq "param") {
                    my $p = $2;
                    $p =~ /^([^ ]+)(?:[ \t]+(.+))?$/;

                    $param{$1} = $2;
                    push(@paramlist, $1);
                }
            }
        }
        close(IN);
    }
}

out("<html>");
out("	<head>");
out("		<meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\">");
out("		<title>$title</title>");
out("	</head>");
out("	<body>");
out("		<h1 align=\"center\">$title</h1>");
out("		<hr>");
out("		<h2 align=\"center\">Table of Contents</h2>");

my $old = $html;
$html = "";
foreach my $f (@pathlist) {
    scan_dir($f, $f);
}
my $stuff = $html;
$html = $old;

out("<dl>");
foreach my $f (@files) {
    out("<dt>");
    out("	<a href=\"#" . replace_special($f) . "\">$f</a>");
    out("</dt>");
    foreach my $sect (@{ $files_sections{ replace_special($f) } }) {
        out("<dd>");
        out(    "	<a href=\"#"
              . replace_special($f . "__" . $sect)
              . "\">$sect</a>");
        out("</dd>");
    }
}
out("</dl>");

out("<hr>");

$html = $html . $stuff;

out("	</body>");
out("</html>");

open(OUT, ">", "doc/index.html");
print(OUT $html);
close(OUT);
