#!/bin/sh
# packs the Retro68 (Classic Mac OS) examples with the images they load into an HFS disk image
# usage: tools/retro68-pack.sh [build directory] [output]
# needs hfsutils (hformat, hmount, hcopy, humount)
set -e
build=${1:-build-ppc}
out=${2:-milsko-examples.dsk}
buildfor=${3:ppc}
folder="Milsko Examples"

rm -f "$out"
dd if=/dev/zero of="$out" bs=1M count=32 2>/dev/null
hformat -l "Milsko" "$out" >/dev/null
hmount "$out" >/dev/null
hmkdir ":$folder"
if [[ $buildfor == ppc ]]; then
for i in "$build"/examples/basic/*.bin "$build"/examples/gldemos/*.bin; do
	case "$i" in
	*.rsrc.bin) ;;
	*) hcopy -m "$i" ":$folder:" ;;
	esac
done
else
for i in "$build"/examples/basic/*[^.code].bin; do
	case "$i" in
	*.rsrc.bin) ;;
	*) hcopy -m "$i" ":$folder:" ;;
	esac
done
fi

# the examples fall back to loading these from their own folder
hcopy -r resource/logo/logo.png ":$folder:logo.png"
hcopy -r examples/picture.jpg ":$folder:picture.jpg"
hcopy -r examples/picture.png ":$folder:picture.png"
hattrib -t PNGf ":$folder:logo.png"
hattrib -t PNGf ":$folder:picture.png"
hattrib -t JPEG ":$folder:picture.jpg"
humount
echo "$out"
