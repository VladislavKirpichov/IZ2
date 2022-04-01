cmake_minimum_required(VERSION 3.11)
project(IZ2 C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES "main.c")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Add ./data files
file(GLOB DATA_FILES "data/*.txt")
file(COPY ${DATA_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

set(FILE "file")
set(VECTOR_STATIC "vector")
set(VECTOR_STATIC_SOURCE_DIR "./include/vector_static")

set(VECTOR_DYNAMIC "vector")
set(VECTOR_DYNAMIC_SOURCE_DIR "./include/vector_dynamic")

set(CMAKE_BUILD_TYPE Debug)

# Static lib
if (static)
    add_subdirectory(${VECTOR_STATIC_SOURCE_DIR} lib)

    target_include_directories(${PROJECT_NAME} PUBLIC ${VECTOR_STATIC_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} ${VECTOR_STATIC})

# Dynamic lib
elseif (dynamic)
    add_subdirectory(${VECTOR_DYNAMIC_SOURCE_DIR} lib)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -lm")
    set(CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${DCMAKE_CC_COMPILER} -pthread")

    target_include_directories(${PROJECT_NAME} PUBLIC ${VECTOR_DYNAMIC_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} ${VECTOR_DYNAMIC})
endif ()