cmake_minimum_required(VERSION 3.15)
project(Cubescape VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(utils)

add_subdirectory(src)
add_subdirectory(third-party)
