#!/bin/sh
#
# on my machine "make distclean" dosn't work correctly. Therefore i use this
# script
find . -name "\.deps" | xargs rm -r
find . -name Makefile | xargs rm
