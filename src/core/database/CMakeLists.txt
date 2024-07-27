cmake_minimum_required(VERSION 3.6)
project(database)

add_library(${PROJECT_NAME}
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} PRIVATE utils)
