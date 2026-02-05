cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME stack)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

project(${PROJECT_NAME} VERSION 1.0
                        DESCRIPTION "Stack"
                        LANGUAGES CXX)


add_executable(test_stack test.cpp)
target_link_libraries(test_stack PRIVATE gtest gtest_main)

enable_testing()
add_test(NAME stack_test COMMAND test_stack)
