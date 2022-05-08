cmake_minimum_required(VERSION 3.5)

project(count_words)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Abseil requires at least C++11
set(CMAKE_CXX_STANDARD 17)

# Process Abseil's CMake build system
add_subdirectory("/home/mark/library/abseil-cpp" "${CMAKE_CURRENT_BINARY_DIR}/absl_build")

add_compile_definitions(USE_ABSL_FLAT_HASH_MAP)
add_executable(count_words_absl_flat_hmap count_words.cpp)

# Declare dependency on the absl::flat_hash_map library
target_link_libraries(count_words_absl_flat_hmap absl::flat_hash_map)
