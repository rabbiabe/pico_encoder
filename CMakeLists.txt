
cmake_minimum_required(VERSION 3.22)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(pico_encoder_example VERSION 1.0.0)
add_executable(${PROJECT_NAME}
                pico_encoder.h
                pico_encoder.cpp
                example/pico_encoder_example.h
                example/pico_encoder_example.cpp)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

pico_sdk_init()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/example/)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
                        pico_stdlib
                        hardware_gpio)
