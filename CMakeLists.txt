cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

# Project definition
project(gaussian-factorise 
    VERSION 1.0
    LANGUAGES CXX
)

# Compiler requirements
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Force Clang with libc++
set(CMAKE_CXX_COMPILER "clang++")

# Basic compiler and linker flags for libc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

# Module-specific settings
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules -fimplicit-modules -fprebuilt-module-path=${CMAKE_BINARY_DIR}/pcm")
    
    # Create directory for precompiled module files
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/pcm)
    
    # Don't try to import std module directly - let Clang handle it implicitly
    add_compile_definitions(_LIBCPP_MODULE_NEEDS_INCLUDE_GUARD)
endif()

# Library: inputParser
add_library(inputParser)
target_sources(inputParser
    PUBLIC
        FILE_SET CXX_MODULES FILES
            ${CMAKE_SOURCE_DIR}/src/utility/inputparser.cppm
        PRIVATE
            ${CMAKE_SOURCE_DIR}/src/utility/inputparser.cpp
)

# Library: gaussianInteger
add_library(gaussianInteger)
target_sources(gaussianInteger
    PUBLIC
        FILE_SET CXX_MODULES FILES
            ${CMAKE_SOURCE_DIR}/src/gaussian_integer/gaussian_integer.cppm
        PRIVATE
            ${CMAKE_SOURCE_DIR}/src/gaussian_integer/gaussian_integer.cpp
)

# Main executable
add_executable(gaussian-factorise ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(gaussian-factorise PRIVATE inputParser gaussianInteger)