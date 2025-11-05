cmake_minimum_required(VERSION 3.15)
project(CruzCore)

file(GLOB_RECURSE CORE_SOURCES src/cruz/core/*.cpp src/cruz/core/*.h)
if(NOT CORE_SOURCES)
    message(FATAL_ERROR "No source files found in cruz-core/src")
endif()

add_library(CruzCore STATIC ${CORE_SOURCES})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_include_directories(CruzCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

get_filename_component(EMSCRIPTEN_INCLUDE "${CMAKE_SOURCE_DIR}/../thirdparty/upstream/emscripten/cache/sysroot/include" ABSOLUTE)
target_include_directories(CruzCore PUBLIC "${EMSCRIPTEN_INCLUDE}")
target_compile_definitions(CruzCore PUBLIC GLFW_INCLUDE_NONE)

if(EMSCRIPTEN)
    target_compile_definitions(CruzCore PUBLIC EMSCRIPTEN)
    add_subdirectory(src/cruz/rendering/backends/webgl)
    target_link_libraries(CruzCore PUBLIC BackendWebGL)
    add_subdirectory(src/cruz/platform/web)
    target_link_libraries(CruzCore PUBLIC WebPlatform)
else()
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/glfw ${CMAKE_BINARY_DIR}/thirdparty/glfw)
    add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/glad ${CMAKE_BINARY_DIR}/thirdparty/glad)
    target_link_libraries(CruzCore PUBLIC glfw glad)
    add_subdirectory(src/cruz/rendering/backends/opengl)
    target_link_libraries(CruzCore PUBLIC BackendOpenGL)
    add_subdirectory(src/cruz/platform/win32)
    target_link_libraries(CruzCore PUBLIC WindowsPlatform)
endif()

add_subdirectory(src/cruz/rendering/renderers/primitive)
target_link_libraries(CruzCore PUBLIC PrimitiveRenderer)

# ZLIB
add_subdirectory(
    ${CMAKE_SOURCE_DIR}/thirdparty/zlib
    ${CMAKE_BINARY_DIR}/thirdparty/zlib
    EXCLUDE_FROM_ALL
)

if(NOT TARGET zlib)
    add_library(zlib ALIAS zlibstatic)
endif()

add_subdirectory(
    ${CMAKE_SOURCE_DIR}/thirdparty/libpng
    ${CMAKE_BINARY_DIR}/thirdparty/libpng
    EXCLUDE_FROM_ALL
)

target_link_libraries(CruzCore PUBLIC zlib libpng)