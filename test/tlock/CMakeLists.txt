# ----- test_tlock -----
add_executable(test_tlock EXCLUDE_FROM_ALL test_tlock.c)
target_link_libraries(test_tlock "${PROJECT_BINARY_DIR}/libutils.a" pthread)
add_test(test_tlock test_tlock)
add_dependencies(check_memcheck test_tlock)
add_dependencies(test_tlock utils)

# ----- threads_tlock -----
add_executable(threads_tlock EXCLUDE_FROM_ALL test_tlock.c)
target_link_libraries(threads_tlock "${PROJECT_BINARY_DIR}/libutils.a" pthread)
add_test(threads_tlock threads_tlock)
add_dependencies(check_threads threads_tlock)
add_dependencies(threads_tlock utils)

