cmake_minimum_required(VERSION 3.30)

project(exercise-8 VERSION 1.0
                   DESCRIPTION "Exercise 8"
                   HOMEPAGE_URL "ya.itslearning.com"
                   LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

enable_testing()

add_executable(test_queue test.cpp)
target_link_libraries(test_queue PRIVATE gtest gmock gtest_main)

add_test(NAME test_queue COMMAND test_queue)
