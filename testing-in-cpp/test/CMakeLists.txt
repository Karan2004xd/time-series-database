cmake_minimum_required(VERSION 3.6)
project(testing_in_cpp_tests)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(run_tests "./my_code_test.cpp" "../my_code.cpp")
target_link_libraries(run_tests ${GTEST_LIBRARIES} pthread)
target_include_directories(run_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
add_test(NAME my_tests COMMAND run_tests)
