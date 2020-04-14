cmake_minimum_required(VERSION 3.7)

project(sigiltest)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")

add_library(sl SHARED IMPORTED)

set(CMAKE_CXX_FLAGS "-Wall -std=c++1z")

add_executable(sigiltest1 ${PROJECT_SOURCE_DIR}/sigiltest1.cpp)
add_executable(sigiltest2 ${PROJECT_SOURCE_DIR}/sigiltest2.cpp)

target_link_libraries(sigiltest1 sigil)
target_link_libraries(sigiltest1 GLESv2)
target_link_libraries(sigiltest2 sigil)
target_link_libraries(sigiltest2 GLESv2)
