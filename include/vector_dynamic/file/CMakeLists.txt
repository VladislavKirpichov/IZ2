cmake_minimum_required(VERSION 3.11)
project(file C)

set(SHARED_MEMORY "../shared_memory")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${DCMAKE_CXX_COMPILER} -lrt")

add_library(${PROJECT_NAME} SHARED ${PROJECT_NAME}.c ${PROJECT_NAME}.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SHARED_MEMORY})