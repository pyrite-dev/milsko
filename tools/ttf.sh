#!/bin/sh
# $Id$

for out in src/text/ttf.c src/text/boldttf.c; do
	echo '/* $Id$ */' > $out
	echo '#include <Mw/Milsko.h>' >> $out
	echo '' >> $out
	echo '#ifdef USE_STB_TRUETYPE' >> $out
done
xxd -n MwBoldTTFData -i resource/mononoki.ttf | sed s/_len/Size/ >> src/text/ttf.c
xxd -n MwTTFData -i resource/mononoki-Bold.ttf | sed s/_len/Size/ >> src/text/boldttf.c
for out in src/text/ttf.c src/text/boldttf.c; do
	echo '#endif' >> $out
done
