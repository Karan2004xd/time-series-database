cmake_minimum_required(VERSION 3.6)
project(observer)

add_library(${PROJECT_NAME} INTERFACE)

target_link_directories(${PROJECT_NAME} INTERFACE "./include")
