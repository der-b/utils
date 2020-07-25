# ----- test_tlock.out -----
add_executable(test_tlock.out EXCLUDE_FROM_ALL test_tlock.c)
target_link_libraries(test_tlock.out "${CMAKE_BINARY_DIR}/libutils.a" pthread)
add_test(test_tlock.out test_tlock.out)
add_dependencies(check_memcheck test_tlock.out)
add_dependencies(test_tlock.out utils)

# ----- threads_tlock.out -----
add_executable(threads_tlock.out EXCLUDE_FROM_ALL test_tlock.c)
target_link_libraries(threads_tlock.out "${CMAKE_BINARY_DIR}/libutils.a" pthread)
add_test(threads_tlock.out threads_tlock.out)
add_dependencies(check_threads threads_tlock.out)
add_dependencies(threads_tlock.out utils)

