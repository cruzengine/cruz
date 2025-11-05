cmake_minimum_required(VERSION 3.10)
project(libpng C)

file(GLOB LIBPNG_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
add_library(libpng STATIC ${LIBPNG_SOURCES})
set_target_properties(libpng PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

target_include_directories(libpng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})