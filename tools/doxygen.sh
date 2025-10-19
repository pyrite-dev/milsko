#!/bin/sh
# $Id$
doxygen
rm -rf /var/www/milsko/doxygen
cp -rf doxygen/doc/html /var/www/milsko/doxygen
