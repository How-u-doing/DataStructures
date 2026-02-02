cmake_minimum_required(VERSION 3.10)

project(PdOps)

set(CMAKE_CXX_STANDARD 17)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CXX_FLAGS
    -Wall
    -Wextra
)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto")

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)

function(add_shared_lib target)
    add_library(${target} MODULE ${ARGN})
    set_target_properties(${target} PROPERTIES PREFIX "")

    target_link_directories(${target} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
    )
    target_link_libraries(${target} PRIVATE
        select_nth_unstable
        pybind11::module
    )
endfunction()

add_shared_lib(ops
    src/ops.cc
    src/statistics.cc
)
