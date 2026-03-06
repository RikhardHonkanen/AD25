cmake_minimum_required(VERSION 3.30)

set(PROJECT_NAME exercise-7)

project(${PROJECT_NAME} VERSION 1.0
                        DESCRIPTION "Exercise 7"
                        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(list_test list/list.cpp test.cpp)
target_include_directories(list_test PRIVATE list)
target_link_libraries(list_test PRIVATE gtest gmock gtest_main)

enable_testing()
add_test(NAME list_test COMMAND list_test)