cmake_minimum_required(VERSION 3.7)

project(sdl2test)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_FLAGS "-Wall")
# -lSDL2 -lSDL2main")
add_executable(sdl2test ${PROJECT_SOURCE_DIR}/sdl2test1.cpp)

find_package(SDL2 REQUIRED)
#find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
#find_library(SDL2_LIBRARY NAME SDL2)

#target_include_directories(sdl2test ${SDL2_INCLUDE_DIR})
#target_link_libraries(sdl2test ${SDL2_LIBRARY})
target_link_libraries(sdl2test ${SDL2_LIBRARIES})
