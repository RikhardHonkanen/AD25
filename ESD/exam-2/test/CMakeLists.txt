cmake_minimum_required(VERSION 3.30)

set(PROJECT_NAME exam)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

project(${PROJECT_NAME} VERSION 1.0
                        DESCRIPTION "Exam"
                        LANGUAGES C)

add_library(unity STATIC unity/unity.c)
target_include_directories(unity PRIVATE unity)

add_library(button STATIC ../lib/button/button.c)
target_include_directories(button PRIVATE test_button)
target_compile_options(button PRIVATE --coverage)

add_executable(button_test test_button/bsp.c test_button/test.c)
target_include_directories(button_test PRIVATE ../lib/button unity test_button)
target_link_libraries(button_test PRIVATE unity button)
target_compile_options(button_test PRIVATE -DDEVENV)
target_link_options(button_test PRIVATE --coverage)

add_custom_target(coverage button_test
   COMMAND gcovr -r ${CMAKE_SOURCE_DIR}/.. --html-details -o ${CMAKE_BINARY_DIR}/index.html
)

add_custom_target(esp_main cd ${CMAKE_SOURCE_DIR}/.. && pio run -t upload)

add_custom_target(esp_test cd ${CMAKE_SOURCE_DIR}/.. && pio test)