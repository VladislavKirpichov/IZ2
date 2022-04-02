cmake_minimum_required(VERSION 3.11)
project(histogram C)

add_library(${PROJECT_NAME} STATIC ${PROJECT_NAME}.c ${PROJECT_NAME}.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})