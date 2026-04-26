cmake_minimum_required(VERSION 3.15)

project(mini_stl
    VERSION 1.0.0
    DESCRIPTION "A lightweight header-only C++ mini STL library"
    LANGUAGES CXX
)

# -------------------------------------------------------
# C++ Standard
# -------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------------
# Library Target (header-only = INTERFACE)
# -------------------------------------------------------
add_library(mini_stl INTERFACE)

target_include_directories(mini_stl INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# -------------------------------------------------------
# Demo / Example executable
# -------------------------------------------------------
add_executable(demo examples/demo.cpp)
target_link_libraries(demo PRIVATE mini_stl)

# -------------------------------------------------------
# Compiler warnings (good practice for a portfolio project)
# -------------------------------------------------------
target_compile_options(demo PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# -------------------------------------------------------
# Install rules
# -------------------------------------------------------
install(TARGETS mini_stl EXPORT mini_stlTargets)
install(DIRECTORY include/ DESTINATION include)
