cmake_minimum_required(VERSION 3.11)
project(IZ2 C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES "main.c")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Add ./data files
file(GLOB DATA_FILES "data/*.txt")
file(COPY ${DATA_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

set(FILE "file")
set(VECTOR "vector")

set(CMAKE_BUILD_TYPE Debug)

# Static lib
if (static)
    set(VECTOR_SOURCE_DIR "./include/vector_static")
    add_subdirectory(${VECTOR_SOURCE_DIR} lib)

    target_include_directories(${PROJECT_NAME} PUBLIC ${VECTOR_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} ${VECTOR})

# Dynamic lib
elseif (dynamic)
    set(VECTOR_SOURCE_DIR "./include/vector_dynamic")
    add_subdirectory(${VECTOR_SOURCE_DIR} lib)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -lm")
    set(CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${DCMAKE_CC_COMPILER} -pthread -fsanitize=address")

    target_include_directories(${PROJECT_NAME} PUBLIC ${VECTOR_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} ${VECTOR})

endif ()

if (sanitaizer_test)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${DCMAKE_CXX_COMPILER} -fsanitize=address -coverage -lgcov -g -Wall -Werror")
    set(CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${DCMAKE_CC_COMPILER} -fsanitize=address")

elseif (test)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LCOV_COMPILE_FLAGS} -pthread -coverage -lgcov -g -Wall -Werror")
    set(CMAKE_CXX_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${LCOV_LINK_FLAGS} -pthread")

endif ()

set(TESTS_DIR "./tests")

if (sanitaizer_test OR test)
    add_subdirectory(${TESTS_DIR})
endif ()
