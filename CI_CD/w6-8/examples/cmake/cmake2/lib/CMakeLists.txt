
add_library(led STATIC led/led.cpp)
target_include_directories(led PUBLIC ../include)

add_library(ldr STATIC ldr/ldr.cpp)
target_include_directories(ldr PRIVATE ../include)