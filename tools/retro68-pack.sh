#!/bin/sh
# packs the Retro68 (Classic Mac OS) examples with the images they load into an HFS disk image
# usage: tools/retro68-pack.sh [build directory] [output]
# needs hfsutils (hformat, hmount, hcopy, humount)
set -e
build=${1:-build-ppc}
out=${2:-milsko-examples.dsk}
folder="Milsko Examples"

rm -f "$out"
dd if=/dev/zero of="$out" bs=1M count=32 2>/dev/null
hformat -l "Milsko" "$out" >/dev/null
hmount "$out" >/dev/null
hmkdir ":$folder"
for i in "$build"/examples/basic/*.bin "$build"/examples/gldemos/*.bin; do
	case "$i" in
	*.rsrc.bin) ;;
	*) hcopy -m "$i" ":$folder:" ;;
	esac
done
# the examples fall back to loading these from their own folder
hcopy -r resource/logo/logo.png ":$folder:logo.png"
hcopy -r examples/picture.jpg ":$folder:picture.jpg"
hcopy -r examples/picture.png ":$folder:picture.png"
hattrib -t PNGf ":$folder:logo.png"
hattrib -t PNGf ":$folder:picture.png"
hattrib -t JPEG ":$folder:picture.jpg"
humount
echo "$out"
