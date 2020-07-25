add_library(utils
    urlpharser/urlpharser.c
    base64/base64.c
    overflow/overflow.c
    log/log.c
    list/list_array.c
    list/list_stlinked.c
    list/list_dlinked.c
    tlock/tlock.c)

target_include_directories(utils INTERFACE "${PROJECT_SOURCE_DIR}/include")
