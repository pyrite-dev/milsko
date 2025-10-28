#!/bin/sh
# $Id$
MAKE=make
if which gmake >/dev/null 2>&1; then
	MAKE=gmake
fi
./tools/icon.sh
./tools/genmk.pl
$MAKE format
