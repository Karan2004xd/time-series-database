cmake_minimum_required(VERSION 3.6)
project(testing_in_cpp)

add_executable(${PROJECT_NAME} "./main.cpp" "./my_code.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(test)
