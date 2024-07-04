
cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(RP2040_Universal_SFP_Transceiver)

pico_sdk_init()

add_executable(${PROJECT_NAME} 
        ./src/main.cpp
        )

target_link_libraries(${PROJECT_NAME} 
        pico_stdlib
        )

pico_add_extra_outputs(${PROJECT_NAME})

