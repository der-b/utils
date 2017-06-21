#!/bin/sh

cppcheck -j 5 --enable=warning,performance,portability --template "{file}({line}):{id}: {message}" --error-exitcode=1 --exitcode-suppressions=cppcheck.suppressions.list.txt -I ../../include ../../
