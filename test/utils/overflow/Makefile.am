# tests which are expected to fail
#XFAIL_TEST =

# uncommend if hard failures should be threated as normal errors
# DISABLE_HARD_ERRORS = 1

#AM_LDFLAGS =
AM_CFLAGS = -g -O0 -Wall

noinst_HEADERS = test_header.h

check_PROGRAMS = test_overflow

TESTS = test_overflow

test_overflow_SOURCES  = test.c
test_overflow_SOURCES += test_header.h
test_overflow_SOURCES += ../../../src/utils/overflow/overflow.c

# disabling oveflow warning, since in this test we provoke a lot of 
# integer over- and underflows intentionally. The caused warnings are
# anoying.
test_overflow_CFLAGS = -Wno-overflow

