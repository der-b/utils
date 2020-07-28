# ----- test_parallel -----
add_executable(test_parallel EXCLUDE_FROM_ALL test_parallel.c)
target_link_libraries(test_parallel "${CMAKE_BINARY_DIR}/libutils.a" pthread)
add_test(test_parallel test_parallel)
add_dependencies(check_memcheck test_parallel)
add_dependencies(test_parallel utils)

# ----- threads_parallel -----
add_executable(threads_parallel EXCLUDE_FROM_ALL test_parallel.c)
target_link_libraries(threads_parallel "${CMAKE_BINARY_DIR}/libutils.a" pthread)
add_test(threads_parallel threads_parallel)
add_dependencies(check_threads threads_parallel)
add_dependencies(threads_parallel utils)

