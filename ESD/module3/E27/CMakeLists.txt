cmake_minimum_required(VERSION 3.22)

project(E27 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(E27 main.cpp)
target_link_libraries(E27 PUBLIC Qt6::Core Qt6::Widgets)
