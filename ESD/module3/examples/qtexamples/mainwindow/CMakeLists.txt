cmake_minimum_required(VERSION 3.22)

project(mainwindow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(mainwindow main.cpp)
target_link_libraries(mainwindow PUBLIC Qt6::Core Qt6::Widgets)
