dnl $Id$
define([space_to_comma],[_space_to_comma(patsubst([$*],[ ],[,]))])dnl
define([_space_to_comma],[ifelse([$#],[0],[],[$#],[1],[$1],[$1[,]_space_to_comma(shift($@))])])dnl
