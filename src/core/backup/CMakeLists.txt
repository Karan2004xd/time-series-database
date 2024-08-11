cmake_minimum_required(VERSION 3.6)
project(backup)

set(SERVICE_COMPONENTS s3)
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})

add_library(${PROJECT_NAME}
  "./src/aws/aws_metadata_handler.cpp"
  "./src/aws/aws_crud_handler.cpp"
  "./src/aws/aws_data_backup.cpp"
)

target_link_directories(${PROJECT_NAME} PRIVATE "./include")
target_link_libraries(${PROJECT_NAME} ${AWSSDK_LINK_LIBRARIES} database)
