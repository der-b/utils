ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src include test
dist_doc_DATA = README.md LICENSE

lib_LIBRARIES = libutils.a
libutils_a_CFLAGS = -DNDEBUG
libutils_a_SOURCES = src/utils/list/list_stlinked.c
libutils_a_SOURCES += src/utils/list/list_array.c
libutils_a_SOURCES += src/utils/list/list_dlinked.c
libutils_a_SOURCES += src/utils/tlock/tlock.c
libutils_a_SOURCES += src/utils/urlpharser/urlpharser.c
libutils_a_SOURCES += src/utils/log/log.c
libutils_a_SOURCES += src/utils/overflow/overflow.c
libutils_a_SOURCES += src/utils/base64/base64.c
