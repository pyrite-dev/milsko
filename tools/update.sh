#!/bin/sh
# $Id$
MAKE=make
if which gmake >/dev/null 2>&1; then
	MAKE=gmake
fi
./tools/doc.pl
./tools/genoo.pl
./tools/genmsvc.pl
$MAKE format
