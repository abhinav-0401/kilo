cmake_minimum_required(VERSION 3.20)
project(kilo)

set(CMAKE_C_STANDARD 17)

add_executable(kilo)

add_subdirectory(src)

# target_sources(kilo PRIVATE
#   "${CMAKE_SOURCE_DIR}/src/"
# )
