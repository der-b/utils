cmake_minimum_required(VERSION 3.10)
project(Utils)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

include_directories("${PROJECT_SOURCE_DIR}/include")

enable_testing()

# add new target check for unittests
set(CTEST_OUTPUT_ON_FAILURE TRUE)
set(MEMORYCHECK_COMMAND_OPTIONS "-q --tool=memcheck --trace-children=yes --leak-check=yes --error-exitcode=1" CACHE STRING "bla")
include(CTest)
add_custom_target(check_memcheck COMMAND ${CMAKE_CTEST_COMMAND} --forec-new-ctest-process --test-action memcheck -R "^test_.*$")

add_custom_target(check_threads COMMAND ${CMAKE_CTEST_COMMAND} --forec-new-ctest-process --test-action memcheck --overwrite MemoryCheckCommandOptions='--tool=helgrind --error-exitcode=1 -q' -R "^threads_.*$")

add_custom_target(check DEPENDS check_memcheck check_threads)

add_subdirectory(src)
add_subdirectory(test)

