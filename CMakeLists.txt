cmake_minimum_required(VERSION 3.6)
project(time_series_database)

add_subdirectory("./src/utils")
add_subdirectory("./src/test")
add_subdirectory("./src/core/observer")
add_subdirectory("./src/core/database")

# set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} "./src/api/main.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC database)
