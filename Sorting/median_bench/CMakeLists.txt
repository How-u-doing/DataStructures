cmake_minimum_required(VERSION 3.14)
project(algos)

find_package(pybind11 REQUIRED)

set(LIBNAME algos)
set(CMAKE_CXX_STANDARD 17)

add_library(${LIBNAME} MODULE src/algos.cc)
set_target_properties(${LIBNAME} PROPERTIES PREFIX "")
target_link_directories(${LIBNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBNAME} PRIVATE pybind11::module select_nth_unstable)
