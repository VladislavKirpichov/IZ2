cmake_minimum_required(VERSION 3.11)
project(vector C)

set(FILE_SOURCE_DIR "file")
set(HISTOGRAM_SOURCE_DIR "histogram")

add_subdirectory(${FILE_SOURCE_DIR} ${FILE_SOURCE_DIR})
add_subdirectory(${HISTOGRAM_SOURCE_DIR} ${HISTOGRAM_SOURCE_DIR})

add_library(${PROJECT_NAME} STATIC ${PROJECT_NAME}.c ${PROJECT_NAME}.h)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} ${FILE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} ${HISTOGRAM_SOURCE_DIR})