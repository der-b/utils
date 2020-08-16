# ----- test_parallel -----
add_executable(test_parallel EXCLUDE_FROM_ALL test_parallel.c)
target_link_libraries(test_parallel "${PROJECT_BINARY_DIR}/libutils.a" pthread)
add_test(test_parallel test_parallel)
add_dependencies(check_memcheck test_parallel)
add_dependencies(test_parallel utils)

# ----- threads_parallel -----
add_executable(threads_parallel EXCLUDE_FROM_ALL test_parallel.c)
target_link_libraries(threads_parallel "${PROJECT_BINARY_DIR}/libutils.a" pthread)
add_test(threads_parallel threads_parallel)
add_dependencies(check_threads threads_parallel)
add_dependencies(threads_parallel utils)

# ----- test_parallel_no_threading -----
add_executable(test_parallel_no_threading EXCLUDE_FROM_ALL
    test_parallel.c
    "${PROJECT_SOURCE_DIR}/src/parallel/parallel.c"
    "${PROJECT_SOURCE_DIR}/src/tlock/tlock.c")
target_compile_options(test_parallel_no_threading PRIVATE -DNO_THREADING)
target_link_libraries(test_parallel_no_threading pthread)
add_test(test_parallel_no_threading test_parallel_no_threading)
add_dependencies(check_memcheck test_parallel_no_threading)

# ----- threads_parallel_no_threading -----
add_executable(threads_parallel_no_threading EXCLUDE_FROM_ALL
    test_parallel.c
    "${PROJECT_SOURCE_DIR}/src/parallel/parallel.c"
    "${PROJECT_SOURCE_DIR}/src/tlock/tlock.c")
target_compile_options(threads_parallel_no_threading PRIVATE -DNO_THREADING)
target_link_libraries(threads_parallel_no_threading pthread)
add_test(threads_parallel_no_threading threads_parallel_no_threading)
add_dependencies(check_threads threads_parallel_no_threading)

# ----- test_parallel_jobs -----
add_executable(test_parallel_jobs EXCLUDE_FROM_ALL test_parallel_jobs.c)
target_link_libraries(test_parallel_jobs "${PROJECT_BINARY_DIR}/libutils.a" pthread)
target_compile_options(test_parallel_jobs PRIVATE -O0 -g)
add_test(test_parallel_jobs test_parallel_jobs)
add_dependencies(check_memcheck test_parallel_jobs)
add_dependencies(test_parallel_jobs utils)

# ----- threads_parallel_jobs -----
add_executable(threads_parallel_jobs EXCLUDE_FROM_ALL test_parallel_jobs.c)
target_link_libraries(threads_parallel_jobs "${PROJECT_BINARY_DIR}/libutils.a" pthread)
target_compile_options(threads_parallel_jobs PRIVATE -O0 -g)
add_test(threads_parallel_jobs threads_parallel_jobs)
add_dependencies(check_memcheck threads_parallel_jobs)
add_dependencies(threads_parallel_jobs utils)
