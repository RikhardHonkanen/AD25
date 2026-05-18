cmake_minimum_required(VERSION 3.30)

set(PROJECT_NAME exercise-24)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)

project(${PROJECT_NAME} VERSION 1.0
                        DESCRIPTION "Exercise 10"
                        LANGUAGES C)

add_library(unity STATIC unity/unity.c)
target_include_directories(unity PRIVATE unity)

add_library(led_driver STATIC ../lib/led_driver/led_driver.c)
target_include_directories(led_driver PRIVATE test_led_driver)
target_compile_options(led_driver PRIVATE --coverage)

add_library(terminal STATIC ../lib/terminal/terminal.c)
target_include_directories(terminal PRIVATE test_terminal)
target_compile_options(terminal PRIVATE --coverage)

add_executable(led_driver_test test_led_driver/bsp.c test_led_driver/test.c)
target_include_directories(led_driver_test PRIVATE ../lib/led_driver unity test_led_driver)
target_link_libraries(led_driver_test PRIVATE unity led_driver)
target_compile_options(led_driver_test PRIVATE -DDEVENV)
target_link_options(led_driver_test PRIVATE --coverage)

add_executable(terminal_test test_terminal/bsp.c test_terminal/led_driver.c test_terminal/test.c)
target_include_directories(terminal_test PRIVATE ../lib/terminal unity test_terminal)
target_link_libraries(terminal_test PRIVATE unity terminal)
target_compile_options(terminal_test PRIVATE -DDEVENV)
target_link_options(terminal_test PRIVATE --coverage)

add_library(led_driver_terminal STATIC ../lib/led_driver/led_driver.c ../lib/terminal/terminal.c)
target_include_directories(led_driver_terminal PRIVATE ../lib/led_driver test_integration)
target_compile_options(led_driver_terminal PRIVATE --coverage)

add_executable(integration_test test_integration/bsp.c test_integration/test.c)
target_include_directories(integration_test PRIVATE ../lib/led_driver unity ../lib/terminal test_integration)
target_link_libraries(integration_test PRIVATE unity led_driver_terminal)
target_compile_options(integration_test PRIVATE -DDEVENV)
target_link_options(integration_test PRIVATE --coverage)

add_custom_target(cov_led_driver led_driver_test
   COMMAND gcovr -r ${CMAKE_SOURCE_DIR}/.. --html-details -o ${CMAKE_BINARY_DIR}/index.html
)

add_custom_target(cov_terminal terminal_test
   COMMAND gcovr -r ${CMAKE_SOURCE_DIR}/.. --html-details -o ${CMAKE_BINARY_DIR}/index.html
)

add_custom_target(cov_integration integration_test
   COMMAND gcovr -r ${CMAKE_SOURCE_DIR}/.. --html-details -o ${CMAKE_BINARY_DIR}/index.html
)

add_custom_target(esp_main cd ${CMAKE_SOURCE_DIR}/.. && pio run -t upload && pio device monitor)

add_custom_target(esp_test cd ${CMAKE_SOURCE_DIR}/.. && pio test)