cmake_minimum_required(VERSION 3.22)
project(laba1 C)

add_executable(my_executable main.c)

target_compile_options(my_executable PRIVATE -O3)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/build
)

add_custom_target(clean
    COMMAND ${CMAKE_COMMAND} -E remove
    ${CMAKE_BINARY_DIR}/my_executable
    ${CMAKE_BINARY_DIR}/CMakeFiles
    ${CMAKE_BINARY_DIR}/*.o
    ${CMAKE_BINARY_DIR}/*.a
    ${CMAKE_BINARY_DIR}/*.so
    ${CMAKE_BINARY_DIR}/*.dylib
)
