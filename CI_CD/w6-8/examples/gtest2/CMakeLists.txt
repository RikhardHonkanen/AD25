cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME stack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${PROJECT_NAME} VERSION 1.0
                        DESCRIPTION "Stack"
                        LANGUAGES CXX)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(stack_test test.cpp)
target_link_libraries(stack_test PRIVATE gtest gtest_main)

enable_testing()
add_test(NAME stack_test COMMAND stack_test)