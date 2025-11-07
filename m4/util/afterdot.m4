dnl $Id$
define([afterdot],[pushdef([rindex])dnl
define([rindex],index([$1],[.]))dnl
define([rindex],ifelse(rindex,[-1],len([$1]),rindex))dnl
substr([$1],[0],rindex)$2[]dnl
popdef([rindex])])dnl
dnl
define([afterdots],[pushdef([first])dnl
define([first],1)dnl
foreach([x],space_to_comma($1),[ifelse(first,[1],[define([first],0)],[ ])afterdot(x,$2)])dnl
popdef([first])])dnl
