cmake_minimum_required(VERSION 3.6)
project(database)

add_library(${PROJECT_NAME}
  "./src/rocksdb_data_repository.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE utils rocksdb)
