cmake_minimum_required(VERSION 3.21)

project(ray_tracing)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB SRC_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

add_executable(ray_tracing ${SRC_FILES})

