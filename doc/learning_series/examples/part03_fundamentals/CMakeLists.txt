cmake_minimum_required(VERSION 3.10)
project(RTTR_Part03_Examples)

# Find RTTR
find_package(rttr CONFIG REQUIRED Core)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Basic types demo
add_executable(basic_types_demo basic_types.cpp)
target_link_libraries(basic_types_demo RTTR::Core)

# Variant demo
add_executable(variant_demo variant_demo.cpp)
target_link_libraries(variant_demo RTTR::Core)

# Simple class example
add_subdirectory(simple_class)