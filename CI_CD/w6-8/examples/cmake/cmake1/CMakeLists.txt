cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

project(${PROJECT_NAME} VERSION 1.0
                        DESCRIPTION "CMake Example"
                        HOMEPAGE_URL "www.project.ulr"
                        LANGUAGES CXX)

add_library(led STATIC lib/led/led.cpp)
target_include_directories(led PRIVATE include)

add_library(ldr STATIC lib/ldr/ldr.cpp)
target_include_directories(ldr PRIVATE include)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE ldr led)
target_include_directories(main PRIVATE lib/ldr lib/led)

add_executable(test_led test/led/test.cpp)
target_link_libraries(test_led PRIVATE led gtest)
target_include_directories(test_led PRIVATE lib/led include)

add_executable(test_ldr test/ldr/test.cpp)
target_link_libraries(test_ldr PRIVATE ldr gtest gtest_main)
target_include_directories(test_ldr PRIVATE lib/ldr include)

enable_testing()
add_test(NAME led_test COMMAND test_led)
add_test(NAME ldr_test COMMAND test_ldr)

add_custom_target(run main 
   COMMAND echo "Done"
)