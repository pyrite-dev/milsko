dnl $Id$
define([space_to_comma],[pushdef([rindex])dnl
define([rindex],index([$*],[ ]))dnl
ifelse(rindex,[-1],[$*],[substr([$*],0,rindex)[,]space_to_comma(substr([$*],eval(rindex + 1)))])[]dnl
popdef([rindex])])dnl
