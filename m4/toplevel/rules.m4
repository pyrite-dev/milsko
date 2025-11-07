dnl $Id$
changequote([,])dnl
include(m4/util/quote.m4)dnl
include(m4/util/arg.m4)dnl
include(m4/util/foreach.m4)dnl
include(m4/util/comma.m4)dnl
include(m4/util/default.m4)dnl
include(m4/util/afterdot.m4)dnl
include(m4/util/my_syscmd.m4)dnl
dnl
default_define([cc],[gcc])dnl
default_define([cflags],[-D_MILSKO])dnl
default_define([incdir],[-I include])dnl
default_define([ldflags],[])dnl
default_define([libdir],[])dnl
default_define([libs],[])dnl
default_define([math],[-lm])dnl
default_define([shared_flag],[-shared])dnl
dnl
define([object_suffix],[.o])dnl
define([library_prefix],[lib])dnl
define([library_suffix],[.so])dnl
define([executable_suffix],[])dnl
dnl
define([library_targets],[])dnl
define([library_objects],[])dnl
define([new_object],[pushdef([object])define([object],afterdots(my_syscmd([sh -c "ls -d $1"]),[object_suffix]))define([library_targets],ifelse(library_targets,[],[object],[library_targets object]))popdef([object])])dnl
define([print_library_targets],[foreach([x],space_to_comma(library_targets),[pushdef([source])define([source],afterdot(x,[.c]))dnl
x: source
	$(CC) -c -fPIC $(CFLAGS) $(INCDIR) -o x source
popdef([source])])])dnl
dnl
define([examples_targets],[])dnl
define([new_example],[define($1[]_cflag,[$2])dnl
define($1[]_ldflag,[$3])dnl
define($1[]_lib,[$4])dnl
define([examples_targets],ifelse(examples_targets,[],[$1[]executable_suffix],[examples_targets $1[]executable_suffix]))])dnl
define([print_examples_targets],[foreach([x],space_to_comma(examples_targets),[pushdef([base])dnl
define([base],afterdot(x,[]))dnl
x: base[]object_suffix library
	$(CC) -L src -Wl,-R./src $(LIBDIR) defn(base[]_ldflag) -o x base[]object_suffix -lMw $(MATH) defn(base[]_lib)
base[]object_suffix: base.c
	$(CC) -c $(INCDIR) -o base[]object_suffix defn(base[]_cflag) base.c

popdef([base])])])dnl
dnl
define([add_incdir],[define([incdir],quote(incdir) [$*])])dnl
define([add_cflag],[define([cflags],quote(cflags) [$*])])dnl
define([add_libdir],[define([libdir],quote(libdir) [$*])])dnl
define([add_ldflag],[define([ldflags],quote(ldflags) [$*])])dnl
define([add_lib],[define([libs],quote(libs) [$*])])dnl
dnl
