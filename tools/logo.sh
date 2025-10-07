#!/bin/sh
# $Id$
povray +W256 +H256 +Ologo.png resource/logo.pov
povray +W200 +H40 +Obanner.png resource/banner.pov
mv logo.png banner.png resource/
