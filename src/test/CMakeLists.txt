cmake_minimum_required(VERSION 3.6)
project(time_series_database_tests)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_SOURCES
  "./utils/json/json_data_builder_test.cpp"
  "./utils/json/simple_json_parser_test.cpp"
  "./utils/logs/logger_test.cpp"
  "./utils/query/simple_query_parser_test.cpp"
  "./utils/query/query_parser_value_test.cpp"

  "./core/database/rocksdb_data_repository_test.cpp"
  "./core/observer/observer_test.cpp"
  "./core/database/horizontal_data_indexer_test.cpp"

  "./core/backup/aws/aws_metadata_handler_test.cpp"
  # "./core/backup/aws/aws_crud_handler_test.cpp"
  "./core/backup/aws/aws_data_backup_test.cpp"
)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests ${GTEST_LIBRARIES} GTest::GTest GTest::Main pthread database backup)
