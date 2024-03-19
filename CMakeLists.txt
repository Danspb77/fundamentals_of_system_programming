cmake_minimum_required(VERSION 3.22)
project(laba1 C)

add_executable(my_executable main.c)

target_compile_options(my_executable PRIVATE -Wall -Wextra -Werror)