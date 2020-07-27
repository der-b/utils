
# ----- test_base64 -----
add_executable(test_base64 EXCLUDE_FROM_ALL test_base64.c)
target_link_libraries(test_base64 "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_base64 test_base64)
add_dependencies(check_memcheck test_base64)
add_dependencies(test_base64 utils)
