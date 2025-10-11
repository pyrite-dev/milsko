#!/bin/sh
# $Id$
for i in *.gif; do
	convert $i -shave 4x4 -alpha on -channel rgba -fill 'transparent' -opaque '#bebebe' `echo $i | cut -d. -f1`.png
done
