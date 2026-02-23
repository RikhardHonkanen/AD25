cmake_minimum_required(VERSION 3.30)

project(exercise-5 VERSION 1.0
                   DESCRIPTION "Exercise 5"
                   LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(queue_test test.cpp)
target_link_libraries(queue_test PRIVATE gtest gmock gtest_main)

enable_testing()
add_test(NAME queue COMMAND queue_test)
