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
add_subdirectory("$ENV{HOME}/library/abseil-cpp" "${CMAKE_CURRENT_BINARY_DIR}/absl_build")

add_executable(count_words_absl_flat_hmap count_words.cpp)
target_compile_definitions(count_words_absl_flat_hmap PUBLIC -DUSE_ABSL_FLAT_HASH_MAP)

# Declare dependency on the absl::flat_hash_map library
target_link_libraries(count_words_absl_flat_hmap absl::flat_hash_map)

# phmap::flat_hash_map
add_executable(count_words_phmap_flat_hmap count_words.cpp)
target_include_directories(count_words_phmap_flat_hmap PUBLIC
                          "$ENV{HOME}/library/parallel-hashmap")
target_compile_definitions(count_words_phmap_flat_hmap PUBLIC -DUSE_PHMAP_FLAT_HASH_MAP)

# robin_hood::unordered_map
add_executable(count_words_robin_hood count_words.cpp)
target_include_directories(count_words_robin_hood PUBLIC
                          "$ENV{HOME}/library/robin-hood-hashing/src/include")
target_compile_definitions(count_words_robin_hood PUBLIC -DUSE_ROBIN_HOOD)
