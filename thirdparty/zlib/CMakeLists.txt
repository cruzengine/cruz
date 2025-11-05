cmake_minimum_required(VERSION 3.10)
project(zlib C)

file(GLOB ZLIB_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
add_library(zlib STATIC ${ZLIB_SOURCES})
set_target_properties(zlib PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

target_include_directories(zlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})