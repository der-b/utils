# ----- test_urlparser -----
add_executable(test_urlparser EXCLUDE_FROM_ALL test_urlparser.c)
target_link_libraries(test_urlparser "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_urlparser test_urlparser)
add_dependencies(check_memcheck test_urlparser)
add_dependencies(test_urlparser utils)
