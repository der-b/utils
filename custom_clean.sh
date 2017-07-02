#!/bin/sh
#
# on my machine "make distclean" dosn't work correctly. Therefore i use this
# script
find . -name "\.deps" | xargs rm -r
find . -name Makefile | xargs rm
rm $(find . -name "*\.o")
rm $(find . -name "Makefile\.in")
rm $(find . -name "*\.out")
rm $(find . -name "*\.log")
rm $(find . -name "*\.trs")
