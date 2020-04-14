cmake_minimum_required(VERSION 3.7)

project(sdl2test)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_FLAGS "-Wall")

add_executable(sdl2test1 ${PROJECT_SOURCE_DIR}/sdl2test1.cpp)
add_executable(sdl2test2 ${PROJECT_SOURCE_DIR}/sdl2test2.cpp)

find_package(SDL2 REQUIRED)
target_link_libraries(sdl2test1 ${SDL2_LIBRARIES})
target_link_libraries(sdl2test2 ${SDL2_LIBRARIES})
