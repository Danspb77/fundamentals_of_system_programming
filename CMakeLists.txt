cmake_minimum_required(VERSION 3.22)
project(laba1 C)

# Добавляем флаги компиляции -Wall, -Wextra, и -Werror
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

add_executable(my_executable main.c)

target_compile_options(my_executable PRIVATE -O3)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory /Documents/OSP/lab1/build
)
