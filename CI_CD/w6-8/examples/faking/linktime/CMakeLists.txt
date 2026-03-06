cmake_minimum_required(VERSION 3.22.3)

set(PROJECT_NAME linktime)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

project(${PROJECT_NAME} VERSION 1.0
                        DESCRIPTION "Link-time"
                        LANGUAGES CXX)

add_executable(test test/date.cpp test/test.cpp)
target_link_libraries(test PRIVATE gtest gtest_main)
target_include_directories(test PRIVATE test today)

add_executable(main main.cpp date/date.cpp)
target_include_directories(main PRIVATE date today)