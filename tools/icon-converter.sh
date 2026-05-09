#!/bin/sh
if [ "x$1" = "x" -o "x$2" = "x" ]; then
	echo "Usage: $0 input name"
	exit 1
fi

GEO=`convert $1 json:- 2>/dev/null | jq -r '(.[0].image.geometry.width | tostring) + "x" + (.[0].image.geometry.height | tostring)'`
WIDTH=`echo $GEO | cut -dx -f1`
HEIGHT=`echo $GEO | cut -dx -f2`

echo "MwU32 $2[] = {"
echo "  ($WIDTH << 16) | $HEIGHT,"
convert $1 txt:- 2>/dev/null | grep -oE '#[0-9a-fA-F]{8}' | sed 's/^#/  0x/' | sed -E 's/$/,/'
echo "  0"
echo "};"
