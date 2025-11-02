#!/bin/sh
# $Id$
povray +W256 +H256 +Ologo.png resource/logo/logo.pov
povray +W200 +H40 +Obanner.png resource/logo/banner.pov
povray +W1280 +H640 +Obanner2.png resource/logo/banner.pov
mv logo.png banner.png banner2.png resource/logo/
magick resource/logo/logo.png -scale x64 resource/logo/logo64.png
