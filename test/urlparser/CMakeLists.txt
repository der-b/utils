# ----- test_urlparser.out -----
add_executable(test_urlparser.out EXCLUDE_FROM_ALL test_urlparser.c)
target_link_libraries(test_urlparser.out "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_urlparser.out test_urlparser.out)
add_dependencies(check_memcheck test_urlparser.out)
add_dependencies(test_urlparser.out utils)
