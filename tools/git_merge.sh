#!/usr/bin/env bash
# $Id$

if [[ $1 -eq "" ]]; then
    echo "Blank argument."
    exit
fi

echo "Applying PR #$1    (https://github.com/pyrite-dev/milsko/pull/$1.diff)"
curl https://github.com/pyrite-dev/milsko/pull/$1.diff -L | patch -p1
