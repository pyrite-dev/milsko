#!/usr/bin/env perl
# $Id$

our $html = "";
our $title = "Milsko GUI Toolkit Documentation";
our @pathlist = ("include");
our @notes = ("warning", "unsure", "note");

sub out {
	$html = $html . "$_[0]\n";
}

sub scan_dir {
	my ($first, $path) = @_;
	$path =~ s/\/+$//g;

	if(opendir(my $dh, $path)){
		print(STDERR "Scanning $path\n");
		my @paths = sort(readdir($dh));
		foreach my $p (@paths){
			if($p eq '.' || $p eq '..'){
				next;
			}
			scan_dir($first, $path . "/" . $p);
		}
		closedir($dh);
	}else{
		my $spaces = "";
		my $in = 0;
		my $out = 0;
		my $file = 0;
		my $has_file = 0;
		my %kv = ();

		print(STDERR "$path is a file\n");
		open(IN, "<", "$path");
		while(my $l = <IN>){
			$l =~ s/[\r\n]+$//g;
			if($out){
				my $para = "p";
				my $brief = $kv{brief} or "";

				$brief =~ s/([^\.])$/\1./g;

				$out = 0;
				if(!$has_file){
					print(STDERR "Warning: missing comment with %file, ignoring\n");
					next;
				}
				if(!$file && defined($kv{brief})){
					$para = "dd";

					$l =~ s/[ \t]*(?:;|\{.+)[ \t]*$//g;

					out("<dt>");
					out("	<code>$l;</code>");
					out("</dt>");
				}
				out("<$para>");
				out("	$brief");
				out("</$para>");

				foreach my $note (@notes){
					if(defined($kv{$note})){
						out("<dt>");
						out("	<img src=\"$note.gif\" alt=\"$note\">");
						out("</dt>");
						out("<dd>");
						out("	$kv{$note}");
						out("</dd>");
					}
				}
				if(!$file){
					out("<br>");
				}
			}elsif($l =~ /^([ \t]*)\/\*\!/){
				$spaces = $1;
				$in = 1;
				$file = 0;
				%kv = ();
			}elsif($in && ($l =~ /^$spaces \*\//)){
				$in = 0;
				$out = 1;
			}elsif($in && ($l =~ /^$spaces \*[ \t]+%([^ ]+)(?:[ \t]+(.+))?$/)){
				$kv{$1} = $2;
				if($1 eq "file"){
					if(!$has_file){
						out("<hr>");
						out("<h2>$2</h2>");
					}

					$file = 1;
					$has_file = 1;
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

foreach my $f (@pathlist){
	scan_dir($f, $f);
}

out("	</body>");
out("</html>");

print($html);
