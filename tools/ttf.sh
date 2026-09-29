#!/bin/sh
for out in src/text/font/ttf.c src/text/font/boldttf.c src/text/font/monottf.c src/text/font/boldmonottf.c; do
	echo '#include <Mw/Milsko.h>' > $out
	echo '' >> $out
	echo '#if defined(USE_STB_TRUETYPE) || defined(USE_FREETYPE2) || defined(USE_GDI_TEXT)' >> $out
done
xxd -n MwMonospaceTTFData -i resource/font/IBMPlexMono-Regular.ttf | sed s/_len/Size/ >> src/text/font/monottf.c
xxd -n MwBoldMonospaceTTFData -i resource/font/IBMPlexMono-Bold.ttf | sed s/_len/Size/ >> src/text/font/boldmonottf.c
xxd -n MwTTFData -i resource/font/OpenSans-Regular.ttf | sed s/_len/Size/ >> src/text/font/ttf.c
xxd -n MwBoldTTFData -i resource/font/OpenSans-Bold.ttf | sed s/_len/Size/ >> src/text/font/boldttf.c
for out in src/text/font/ttf.c src/text/font/boldttf.c src/text/font/monottf.c src/text/font/boldmonottf.c; do
	echo '#endif' >> $out
done
