cmake_minimum_required(VERSION 3.20)
project(Kilo)

set(CMAKE_C_STANDARD 17)

add_executable(Kilo)

add_subdirectory(src)

# target_sources(kilo PRIVATE
#   "${CMAKE_SOURCE_DIR}/src/"
# )
