add_library(utils
    urlparser/urlparser.c
    base64/base64.c
    overflow/overflow.c
    log/log.c
    list/list_array.c
    list/list_stlinked.c
    list/list_dlinked.c
    tlock/tlock.c
    parallel/parallel.c)

target_include_directories(utils INTERFACE "${PROJECT_SOURCE_DIR}/include")
