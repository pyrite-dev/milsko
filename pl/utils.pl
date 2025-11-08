# $Id$
sub add_incdir {
	my $input = $_[0];
	$input =~ s/\r?\n/ /g;

	$incdir = "${incdir} ${input}";
}

sub add_cflags {
	my $input = $_[0];
	$input =~ s/\r?\n/ /g;

	$cflags = "${cflags} ${input}";
}

sub add_libdir {
	my $input = $_[0];
	$input =~ s/\r?\n/ /g;

	$libdir = "${libdir} ${input}";
}

sub add_ldflags {
	my $input = $_[0];
	$input =~ s/\r?\n/ /g;

	$ldflags = "${ldflags} ${input}";
}

sub add_libs {
	my $input = $_[0];
	$input =~ s/\r?\n/ /g;

	$libs = "${libs} ${input}";
}

sub new_example {
	if(@_ == 2){
		$examples_libs{"$_[0]${executable_suffix}"} = $_[1];
	}
	push(@examples_targets, "${_[0]}${executable_suffix}");
}

sub new_object {
	my @l = glob($_[0]);
	foreach my $e (@l){
		$e =~ s/\.c$/$object_suffix/;
		push(@library_targets, $e);
	}
}

sub use_backend {
	$backend = $_[0];
}

our %params = ();
sub param_set {
	$params{$_[0]} = $_[1];
}

sub param_get {
	if(not(defined($params{$_[0]}))){
		return 0;
	}else{
		return $params{$_[0]};
	}
}

sub param_list {
	return sort(keys(%params));
}

1;
