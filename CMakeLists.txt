cmake_minimum_required(VERSION 3.21)
project(t3new)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(t3new main.cpp)
target_link_libraries(t3new ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY})