#!/bin/sh
set -e
wmake -f WatMakefile
rm -rf milsko
mkdir -p milsko-examples
cp src/Mw.dll milsko-examples/
cd examples/basic
for i in *.c; do
	owcc -bnt -I../../include ../../src/Mw.lib -o../../milsko-examples/`echo $i | cut -d. -f1`.exe $i
done
cd ../..
cd examples/gldemos
for i in *.c; do
	if [ "$i" = "glutlayer.c" ]; then
		continue
	fi
	owcc -bnt -I../../include ../../src/Mw.lib -o../../milsko-examples/gl`echo $i | cut -d. -f1`.exe $i opengl32.lib glu32.lib
done
cd ../..
rm -f milsko-examples.zip
zip -rv milsko-examples.zip milsko-examples
rm -rf milsko-examples
