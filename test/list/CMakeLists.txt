add_compile_options(-g)
# ----- test_list_array -----
add_executable(test_list_array EXCLUDE_FROM_ALL
    array.c
    unittest.c
    ut_newelement.c
    ut_state.c)
target_link_libraries(test_list_array "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_array test_list_array)
add_dependencies(check_memcheck test_list_array)
add_dependencies(test_list_array utils)

# ----- test_list_array -----
add_executable(test_list_array_equal EXCLUDE_FROM_ALL
    array_ut_equal.c
    ut_equal.c
    ut_newelement.c
    ut_state.c)
target_link_libraries(test_list_array_equal "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_array_equal test_list_array_equal)
add_dependencies(check_memcheck test_list_array_equal)
add_dependencies(test_list_array_equal utils)

# ----- test_list_stlinked -----
add_executable(test_list_stlinked EXCLUDE_FROM_ALL
    stlinked.c
    unittest.c
    ut_newelement.c
    ut_state.c)
target_link_libraries(test_list_stlinked "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_stlinked test_list_stlinked)
add_dependencies(check_memcheck test_list_stlinked)
add_dependencies(test_list_stlinked utils)

# ----- test_list_stlinked -----
add_executable(test_list_stlinked_equal EXCLUDE_FROM_ALL
    stlinked_ut_equal.c
    ut_equal.c
    ut_newelement.c
    ut_state.c)
target_link_libraries(test_list_stlinked_equal "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_stlinked_equal test_list_stlinked_equal)
add_dependencies(check_memcheck test_list_stlinked_equal)
add_dependencies(test_list_stlinked_equal utils)

# ----- test_list_dlinked -----
add_executable(test_list_dlinked EXCLUDE_FROM_ALL
    dlinked.c
    unittest.c
    ut_newelement.c
    ut_state.c)
target_link_libraries(test_list_dlinked "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_dlinked test_list_dlinked)
add_dependencies(check_memcheck test_list_dlinked)
add_dependencies(test_list_dlinked utils)

# ----- test_list_dlinked -----
add_executable(test_list_dlinked_equal EXCLUDE_FROM_ALL
    dlinked_ut_equal.c
    ut_equal.c
    ut_state.c)
target_link_libraries(test_list_dlinked_equal "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_dlinked_equal test_list_dlinked_equal)
add_dependencies(check_memcheck test_list_dlinked_equal)
add_dependencies(test_list_dlinked_equal utils)

# ----- test_list_array_prealloc -----
add_executable(test_list_array_prealloc EXCLUDE_FROM_ALL
    array.c
    unittest.c
    ut_newelement.c
    ut_state.c)
target_compile_options(test_list_array_prealloc PRIVATE -D_LIST_PREALLOC)
target_link_libraries(test_list_array_prealloc "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_array_prealloc test_list_array_prealloc)
add_dependencies(check_memcheck test_list_array_prealloc)
add_dependencies(test_list_array_prealloc utils)

# ----- test_list_array_prealloc -----
add_executable(test_list_array_equal_prealloc EXCLUDE_FROM_ALL
    array_ut_equal.c
    ut_equal.c
    ut_state.c)
target_compile_options(test_list_array_equal_prealloc PRIVATE -D_LIST_PREALLOC)
target_link_libraries(test_list_array_equal_prealloc "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_array_equal_prealloc test_list_array_equal_prealloc)
add_dependencies(check_memcheck test_list_array_equal_prealloc)
add_dependencies(test_list_array_equal_prealloc utils)

# ----- test_list_stlinked_prealloc -----
add_executable(test_list_stlinked_prealloc EXCLUDE_FROM_ALL
    stlinked.c
    unittest.c
    ut_newelement.c
    ut_state.c)
target_compile_options(test_list_stlinked_prealloc PRIVATE -D_LIST_PREALLOC)
target_link_libraries(test_list_stlinked_prealloc "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_stlinked_prealloc test_list_stlinked_prealloc)
add_dependencies(check_memcheck test_list_stlinked_prealloc)
add_dependencies(test_list_stlinked_prealloc utils)

# ----- test_list_stlinked_prealloc -----
add_executable(test_list_stlinked_equal_prealloc EXCLUDE_FROM_ALL
    stlinked_ut_equal.c
    ut_equal.c
    ut_state.c)
target_compile_options(test_list_stlinked_equal_prealloc PRIVATE -D_LIST_PREALLOC)
target_link_libraries(test_list_stlinked_equal_prealloc "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_stlinked_equal_prealloc test_list_stlinked_equal_prealloc)
add_dependencies(check_memcheck test_list_stlinked_equal_prealloc)
add_dependencies(test_list_stlinked_equal_prealloc utils)

# ----- test_list_dlinked_prealloc -----
add_executable(test_list_dlinked_prealloc EXCLUDE_FROM_ALL
    dlinked.c
    unittest.c
    ut_newelement.c
    ut_state.c)
target_compile_options(test_list_dlinked_prealloc PRIVATE -D_LIST_PREALLOC)
target_link_libraries(test_list_dlinked_prealloc "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_dlinked_prealloc test_list_dlinked_prealloc)
add_dependencies(check_memcheck test_list_dlinked_prealloc)
add_dependencies(test_list_dlinked_prealloc utils)

# ----- test_list_dlinked_prealloc -----
add_executable(test_list_dlinked_equal_prealloc EXCLUDE_FROM_ALL
    dlinked_ut_equal.c
    ut_equal.c
    ut_state.c)
target_compile_options(test_list_dlinked_equal_prealloc PRIVATE -D_LIST_PREALLOC)
target_link_libraries(test_list_dlinked_equal_prealloc "${PROJECT_BINARY_DIR}/libutils.a")
add_test(test_list_dlinked_equal_prealloc test_list_dlinked_equal_prealloc)
add_dependencies(check_memcheck test_list_dlinked_equal_prealloc)
add_dependencies(test_list_dlinked_equal_prealloc utils)
