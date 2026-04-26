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
# Compiler warnings
# -------------------------------------------------------
set(MINI_STL_WARNINGS
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# -------------------------------------------------------
# Demo executable
# -------------------------------------------------------
add_executable(demo examples/demo.cpp)
target_link_libraries(demo PRIVATE mini_stl)
target_compile_options(demo PRIVATE ${MINI_STL_WARNINGS})

# Link pthreads on Linux/Mac (required for std::thread)
find_package(Threads REQUIRED)
target_link_libraries(demo PRIVATE Threads::Threads)

# -------------------------------------------------------
# Benchmark executable
# -------------------------------------------------------
add_executable(benchmark benchmarks/benchmark.cpp)
target_link_libraries(benchmark PRIVATE mini_stl Threads::Threads)

# Optimized build for benchmarks
target_compile_options(benchmark PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-O2>
    $<$<CXX_COMPILER_ID:MSVC>:/O2>
)

# -------------------------------------------------------
# Install rules
# -------------------------------------------------------
install(TARGETS mini_stl EXPORT mini_stlTargets)
install(DIRECTORY include/ DESTINATION include)
