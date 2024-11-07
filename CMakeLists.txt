cmake_minimum_required(VERSION 3.20)

project(calc VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)
add_subdirectory(libcalc)

add_executable(calc calc.cpp)
target_compile_options(calc PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(calc PRIVATE libcalc)
target_link_libraries(calc PRIVATE fmt)
