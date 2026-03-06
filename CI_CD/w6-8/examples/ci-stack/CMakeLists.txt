cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_NAME stack)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

project(${PROJECT_NAME} VERSION 1.0
                        DESCRIPTION "Stack"
                        LANGUAGES CXX)

#include FetchContent to be able to download GoogleTest code.
include(FetchContent)

# Declare git repo and tag to fetch:
FetchContent_Declare(
   googletest
   GIT_REPOSITORY https://github.com/google/googletest.git
   GIT_TAG v1.15.2
)

#Download or check that googletest is downloaded:
FetchContent_MakeAvailable(googletest)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE gtest_main gmock_main)