dnl $Id$
define([my_syscmd],[syscmd([$1 > syscmd.out])include(syscmd.out)syscmd([sh -c "rm -f syscmd.out"])])dnl
