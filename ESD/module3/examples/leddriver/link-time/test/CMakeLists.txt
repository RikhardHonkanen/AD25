cmake_minimum_required(VERSION 3.30)

set(PROJECT_NAME led-driver)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)

project(${PROJECT_NAME} VERSION 1.0
                        DESCRIPTION "LED Driver"
                        LANGUAGES C)


include_directories(unity ../lib/led_driver test_led_driver)

add_library(unity STATIC unity/unity.c)

add_library(led_driver STATIC ../lib/led_driver/led_driver.c)
target_compile_options(led_driver PRIVATE --coverage)

add_executable(led_driver_test test_led_driver/driver/gpio.c test_led_driver/test.c)
target_link_libraries(led_driver_test PRIVATE unity led_driver)
target_link_options(led_driver_test PRIVATE --coverage)

add_custom_target(coverage led_driver_test
   COMMAND gcovr -r ${CMAKE_SOURCE_DIR}/.. --html-details -o ${CMAKE_BINARY_DIR}/index.html
)