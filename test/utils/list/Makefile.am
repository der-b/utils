# tests which are expected to fail
#XFAIL_TEST =

# uncommend if hard failures should be threated as normal errors
# DISABLE_HARD_ERRORS = 1

#AM_LDFLAGS =
AM_CFLAGS = -g -O0 -Wall

FILES = 
FILES += unittest.c
FILES += ut_state.c
#FILES += ${top_srcdir}/src/utils/list/list_array.c
#FILES += ${top_srcdir}/src/utils/list/list_stlinked.c
#FILES += ${top_srcdir}/src/utils/list/list_dlinked.c
FILES += ../../../src/utils/list/list_array.c
FILES += ../../../src/utils/list/list_stlinked.c
FILES += ../../../src/utils/list/list_dlinked.c
FILES += ../../../src/utils/overflow/overflow.c

noinst_HEADERS = unittest.h

#AM_CFLAGS = -Werror -Wall -I$(top_srcdir)/include

check_PROGRAMS = array.out
check_PROGRAMS += array_ut_equal.out
check_PROGRAMS += stlinked.out
check_PROGRAMS += stlinked_ut_equal.out
check_PROGRAMS += dlinked.out
check_PROGRAMS += dlinked_ut_equal.out

TESTS = array.out
TESTS += stlinked.out
TESTS += dlinked.out
TESTS += stlinked_ut_equal.out
TESTS += dlinked_ut_equal.out
TESTS += array_ut_equal.out

array_out_SOURCES = array.c $(FILES)
stlinked_out_SOURCES = stlinked.c $(FILES)
dlinked_out_SOURCES = dlinked.c $(FILES)
stlinked_ut_equal_out_SOURCES = stlinked_ut_equal.c ut_equal.c $(FILES)
dlinked_ut_equal_out_SOURCES = dlinked_ut_equal.c ut_equal.c $(FILES)
array_ut_equal_out_SOURCES = array_ut_equal.c ut_equal.c $(FILES)

