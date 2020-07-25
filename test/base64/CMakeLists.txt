
# ----- test_base64.out -----
add_executable(test_base64.out EXCLUDE_FROM_ALL test_base64.c)
target_link_libraries(test_base64.out "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_base64.out test_base64.out)
add_dependencies(check_memcheck test_base64.out)
add_dependencies(test_base64.out utils)
