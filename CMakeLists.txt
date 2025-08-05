cmake_minimum_required(VERSION 3.10)

project(sigma-vm LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(sigma-vm ${SOURCES})

target_include_directories(sigma-vm
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(sigma-vm
	PRIVATE
		-Wall
		-Wextra
		-O2
)
