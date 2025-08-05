cmake_minimum_required(VERSION 3.10)
project(sigma-vm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_compile_options(-Wall -Wextra)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(my_program ${SOURCES})
