cmake_minimum_required(VERSION 3.6)
project(utils)

find_package(Boost REQUIRED COMPONENTS log log_setup)
include_directories(${Boost_INCLUDE_DIR})

add_library(${PROJECT_NAME}
  "./src/json/json_data_builder.cpp"
  "./src/json/json_parser.cpp"
  "./src/logs/Logger.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::log Boost::log_setup)
