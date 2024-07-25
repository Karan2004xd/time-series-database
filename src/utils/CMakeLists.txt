cmake_minimum_required(VERSION 3.6)
project(utils)

add_library(${PROJECT_NAME}
  "./src/json/json_data_builder.cpp"
  "./src/json/json_parser.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
