#!/bin/sh
# $Id$
MAKE=make
if which gmake >/dev/null 2>&1; then
	MAKE=gmake
fi
./tools/doc.pl
./tools/icon.sh
./tools/genoo.pl
./tools/genmk.pl
$MAKE format
