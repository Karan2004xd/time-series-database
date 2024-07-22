cmake_minimum_required(VERSION 3.6)
project(test-grpc)

add_executable(${PROJECT_NAME} "./test_grpc.cpp")
target_link_libraries(${PROJECT_NAME} grpc++)
