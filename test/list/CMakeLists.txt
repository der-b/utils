# ----- test_list_array.out -----
add_executable(test_list_array.out EXCLUDE_FROM_ALL
    array.c
    unittest.c
    ut_state.c)
target_link_libraries(test_list_array.out "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_list_array.out test_list_array.out)
add_dependencies(check_memcheck test_list_array.out)
add_dependencies(test_list_array.out utils)

# ----- test_list_array.out -----
add_executable(test_list_array_equal.out EXCLUDE_FROM_ALL
    array_ut_equal.c
    ut_equal.c
    ut_state.c)
target_link_libraries(test_list_array_equal.out "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_list_array_equal.out test_list_array_equal.out)
add_dependencies(check_memcheck test_list_array_equal.out)
add_dependencies(test_list_array_equal.out utils)

# ----- test_list_stlinked.out -----
add_executable(test_list_stlinked.out EXCLUDE_FROM_ALL
    stlinked.c
    unittest.c
    ut_state.c)
target_link_libraries(test_list_stlinked.out "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_list_stlinked.out test_list_stlinked.out)
add_dependencies(check_memcheck test_list_stlinked.out)
add_dependencies(test_list_stlinked.out utils)

# ----- test_list_stlinked.out -----
add_executable(test_list_stlinked_equal.out EXCLUDE_FROM_ALL
    stlinked_ut_equal.c
    ut_equal.c
    ut_state.c)
target_link_libraries(test_list_stlinked_equal.out "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_list_stlinked_equal.out test_list_stlinked_equal.out)
add_dependencies(check_memcheck test_list_stlinked_equal.out)
add_dependencies(test_list_stlinked_equal.out utils)

# ----- test_list_dlinked.out -----
add_executable(test_list_dlinked.out EXCLUDE_FROM_ALL
    dlinked.c
    unittest.c
    ut_state.c)
target_link_libraries(test_list_dlinked.out "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_list_dlinked.out test_list_dlinked.out)
add_dependencies(check_memcheck test_list_dlinked.out)
add_dependencies(test_list_dlinked.out utils)

# ----- test_list_dlinked.out -----
add_executable(test_list_dlinked_equal.out EXCLUDE_FROM_ALL
    dlinked_ut_equal.c
    ut_equal.c
    ut_state.c)
target_link_libraries(test_list_dlinked_equal.out "${CMAKE_BINARY_DIR}/libutils.a")
add_test(test_list_dlinked_equal.out test_list_dlinked_equal.out)
add_dependencies(check_memcheck test_list_dlinked_equal.out)
add_dependencies(test_list_dlinked_equal.out utils)
