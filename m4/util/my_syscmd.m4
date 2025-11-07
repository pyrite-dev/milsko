dnl $Id$
define([my_syscmd],[syscmd([$1 > my_syscmd_out.out])include(my_syscmd_out.out)syscmd([sh -c "rm -f my_syscmd_out.out"])])dnl
