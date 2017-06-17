# tests which are expected to fail
XFAIL_TEST =

# uncommend if hard failures should be threated as normal errors
# DISABLE_HARD_ERRORS = 1

AM_LDFLAGS =
AM_CFLAGS = -g -O0 -Wall -I$(top_srcdir)/include -pthread

noinst_HEADERS = test_header.h

check_PROGRAMS = test_tlock.out

TESTS = test_tlock.out

test_tlock_out_SOURCES  = test_tlock.c
test_tlock_out_SOURCES += ../../../src/utils/tlock/tlock.c

