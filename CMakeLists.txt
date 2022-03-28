cmake_minimum_required(VERSION 3.11)
project(IZ2 C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES "main.c")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set(FILE "file")
set(VECTOR_STATIC "vector")
set(VECTOR_STATIC_SOURCE_DIR "./include/static")

set(VECTOR_DYNAMIC "vector_dynamic")
set(VECTOR_DYNAMIC_SOURCE_DIR "./include/dynamic")

set(CMAKE_BUILD_TYPE Debug)

# Static lib
if (static)
    add_subdirectory(${VECTOR_STATIC_SOURCE_DIR} lib)

    target_include_directories(${PROJECT_NAME} PUBLIC ${VECTOR_STATIC_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} ${FILE} ${VECTOR_STATIC})

# Dynamic lib
elseif (dynamic)
    add_library(${VECTOR_DYNAMIC} SHARED ${VECTOR_DYNAMIC_SOURCE_DIR}/vector.c ${VECTOR_DYNAMIC_SOURCE_DIR}/vector.h)

    target_include_directories(${PROJECT_NAME} PUBLIC ${VECTOR_DYNAMIC_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} ${VECTOR_DYNAMIC})
endif ()