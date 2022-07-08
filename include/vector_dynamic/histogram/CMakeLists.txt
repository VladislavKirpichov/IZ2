cmake_minimum_required(VERSION 3.11)
project(histogram C)

set(SHARED_MEMORY "../shared_memory")

add_library(${PROJECT_NAME} SHARED ${PROJECT_NAME}.c ${PROJECT_NAME}.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SHARED_MEMORY})