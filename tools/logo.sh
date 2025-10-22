#!/bin/sh
# $Id$
povray +W256 +H256 +Ologo.png resource/logo.pov
povray +W200 +H40 +Obanner.png resource/banner.pov
povray +W1280 +H640 +Obanner2.png resource/banner.pov
mv logo.png banner.png banner2.png resource/
magick resource/logo.png -scale x64 resource/logo64.png
