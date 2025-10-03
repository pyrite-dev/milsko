#!/bin/sh
# $Id$

for i in linux win32 win64; do
	rm -rf ../milsko-$i
	cp -rf . ../milsko-$i
	rm -rf ../milsko-$i/.svn
done
