#!/bin/sh
# $Id$

cd resource/icon
for i in *.gif; do
	if [ ! -f "`echo $i | cut -d. -f1`.png" ]; then
		convert $i -shave 4x4 -alpha on -channel rgba -fill 'transparent' -opaque '#80ff80' `echo $i | cut -d. -f1`.png
	fi
done
cd ../..

echo '/* $Id$ */' > include/Mw/Icon.h
echo '/*!' >> include/Mw/Icon.h
echo ' * %file Mw/Icon.h' >> include/Mw/Icon.h
echo ' * %brief Icon' >> include/Mw/Icon.h
echo ' */' >> include/Mw/Icon.h
echo '#ifndef __MW_ICON_H__' >> include/Mw/Icon.h
echo '#define __MW_ICON_H__' >> include/Mw/Icon.h
echo '' >> include/Mw/Icon.h
echo '#include <Mw/MachDep.h>' >> include/Mw/Icon.h
echo '' >> include/Mw/Icon.h
echo '#ifdef __cplusplus' >> include/Mw/Icon.h
echo 'extern "C" {' >> include/Mw/Icon.h
echo '#endif' >> include/Mw/Icon.h
echo '' >> include/Mw/Icon.h
for i in resource/icon/*.png; do
	LOWER=`echo $i | rev | cut -d"/" -f1 | rev | cut -d"." -f1`
	if [ "$LOWER" = "unsure" ]; then
		LOWER=question
	fi
	OUT=src/icon/$LOWER.c
	NAME=`echo -n $LOWER | perl -e 'my $l = <>;$l =~ s/^(.)/uc($1)/e;print($l);'`
	NAME=MwIcon$NAME
	echo '/* $Id$ */' > $OUT
	echo '#include <Mw/Milsko.h>' >> $OUT
	echo >> $OUT
	convert $i xpm:- 2>/dev/null | sed -E 's/^static //' | sed 's/xpm__/'$NAME'/' >> $OUT
	echo $NAME
done | while read a; do
	echo '/*!' >> include/Mw/Icon.h
	echo " * %brief `echo $a | sed s/MwIcon//` icon" >> include/Mw/Icon.h
	echo ' */' >> include/Mw/Icon.h
	echo "MWDECL char* $a[];" >> include/Mw/Icon.h
	echo '' >> include/Mw/Icon.h
done
echo '' >> include/Mw/Icon.h
echo '#ifdef __cplusplus' >> include/Mw/Icon.h
echo '}' >> include/Mw/Icon.h
echo '#endif' >> include/Mw/Icon.h
echo '' >> include/Mw/Icon.h
echo '#endif' >> include/Mw/Icon.h
