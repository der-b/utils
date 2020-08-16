# ----- test_overflow -----
add_executable(test_overflow EXCLUDE_FROM_ALL test_overflow.c)
# intentionally we create a lot of overflows in this test,
# therefore we disable the compiler warnings
target_compile_options(test_overflow PRIVATE -Wno-overflow)
target_link_libraries(test_overflow "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_overflow test_overflow)
add_dependencies(check_memcheck test_overflow)
add_dependencies(test_overflow utils)
