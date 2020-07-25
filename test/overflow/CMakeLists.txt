# ----- test_overflow.out -----
add_executable(test_overflow.out EXCLUDE_FROM_ALL test_overflow.c)
# intentionally we create a lot of overflows in this test,
# therefore we disable the compiler warnings
target_compile_options(test_overflow.out PRIVATE -Wno-overflow)
target_link_libraries(test_overflow.out "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_overflow.out test_overflow.out)
add_dependencies(check_memcheck test_overflow.out)
add_dependencies(test_overflow.out utils)
