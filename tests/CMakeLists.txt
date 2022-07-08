cmake_minimum_required(VERSION 3.0)
project(tests)

set(CMAKE_CXX_STANDARD 11)

file(GLOB STATIC_SOURCES static/*.cpp)
file(GLOB DYNAMIC_SOURCES dynamic/*.cpp)

find_package(GTest REQUIRED)

file(GLOB TEST_FILES "^test$.txt")
file(GLOB ANSWER_FILES "^answer$.txt")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input/${TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/input)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/answers/${ANSWER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/answers)

if (dynamic) 
    add_executable(${PROJECT_NAME} ${DYNAMIC_SOURCES})

else()
    add_executable(${PROJECT_NAME} ${STATIC_SOURCES})
    
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${VECTOR} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
