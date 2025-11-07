dnl $Id$
define([my_syscmd],[syscmd([$* | tr '\n' ' ' | sed 's/ $//' > my_syscmd_out.out])dnl
include(my_syscmd_out.out)[]syscmd([sh -c "rm -f my_syscmd_out.out"])])dnl
