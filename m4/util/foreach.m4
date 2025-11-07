dnl $Id$
define([foreach],[pushdef([$1])_foreach($@)popdef([$1])])dnl
define([_foreach],[ifelse(quote($2),[],[],[define([$1],[arg1($2)])$3[]_foreach([$1],[shift($2)],[$3])])])dnl
