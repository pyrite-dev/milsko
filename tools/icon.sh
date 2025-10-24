#!/bin/sh
# $Id$

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
	WIDTH=`convert $i json:- 2>/dev/null | jq '.[0].image.geometry.width'`
	HEIGHT=`convert $i json:- 2>/dev/null | jq '.[0].image.geometry.height'`
	echo '/* $Id$ */' > $OUT
	echo '#include <Mw/Milsko.h>' >> $OUT
	echo >> $OUT
	echo "unsigned int $NAME[] = {" >> $OUT
	echo "	($WIDTH << 16) | $HEIGHT," >> $OUT
	convert $i txt:- 2>/dev/null | grep -oE '#[0-9a-fA-F]{8}' | sed 's/#/	0x/' | sed -E 's/$/,/' >> $OUT
	echo "	0" >> $OUT
	echo "};" >> $OUT
	echo $NAME
done | while read a; do
	echo '/*!' >> include/Mw/Icon.h
	echo " * %brief `echo $a | sed s/MwIcon//` icon" >> include/Mw/Icon.h
	echo ' */' >> include/Mw/Icon.h
	echo "MWDECL unsigned int $a[];" >> include/Mw/Icon.h
	echo '' >> include/Mw/Icon.h
done
echo '' >> include/Mw/Icon.h
echo '#ifdef __cplusplus' >> include/Mw/Icon.h
echo '}' >> include/Mw/Icon.h
echo '#endif' >> include/Mw/Icon.h
echo '' >> include/Mw/Icon.h
echo '#endif' >> include/Mw/Icon.h
