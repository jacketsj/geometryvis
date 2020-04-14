cmake_minimum_required(VERSION 3.7)

project(sigiltest)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")

#find_library(sl sigil)
#add_library(sl SHARED IMPORTED)
add_library(sl SHARED IMPORTED)

set(CMAKE_CXX_FLAGS "-Wall -std=c++1z")

add_executable(sigiltest1 ${PROJECT_SOURCE_DIR}/sigiltest1.cpp)

#set_target_properties(sl PROPERTIES IMPORTED

#find_package(sigil REQUIRED)
#include_directories(sdl2test3 ${SDL2_INCLUDE_DIRS})
target_link_libraries(sigiltest1 sigil)
target_link_libraries(sigiltest1 GLESv2)
#set_target_properties(sl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/sigil-linux-gcc-0.9.0/sigil-linux-gcc/libsigil.so)
#target_link_libraries(sigiltest1 sl)
