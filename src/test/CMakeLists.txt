cmake_minimum_required(VERSION 3.6)
project(time_series_database_tests)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_SOURCES
  "./json/json_data_builder_test.cpp"
  "./json/json_parser_test.cpp"
)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests ${GTEST_LIBRARIES} GTest::GTest GTest::Main pthread utils)
