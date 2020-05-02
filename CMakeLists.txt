cmake_minimum_required(VERSION 3.7)

project(geometryvis)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")

add_library(sl SHARED IMPORTED)

set(CMAKE_CXX_FLAGS "-Wall -std=c++1z")

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")

#add_executable(geovis ${PROJECT_SOURCE_DIR}/geovis.cpp ${PROJECT_SOURCE_DIR}/col.cpp ${PROJECT_SOURCE_DIR}/line.cpp ${PROJECT_SOURCE_DIR}/circle.cpp ${PROJECT_SOURCE_DIR}/properties.cpp ${PROJECT_SOURCE_DIR}/render_adapter.cpp ${PROJECT_SOURCE_DIR}/console.cpp)
#add_executable(geovis ${PROJECT_SOURCE_DIR}/geovis.cpp)
add_executable(geovis ${SOURCES})
#target_link_libraries(geovis
#	col.cpp
#	line.cpp
#	circle.cpp
#	properties.cpp
#	render_adapter.cpp
#	console.cpp)

#target_link_libraries(geovis glfw)
target_link_libraries(geovis sigil)
target_link_libraries(geovis GLESv2)
