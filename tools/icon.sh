#!/bin/sh
# $Id$
for i in doc/*.gif; do
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
	convert $i xpm:- | sed -E 's/^static //' | sed 's/xpm__/'$NAME'/' >> $OUT
done
