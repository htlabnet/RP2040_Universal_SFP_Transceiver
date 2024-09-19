
cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(RP2040_Universal_SFP_Transceiver)

pico_sdk_init()

add_executable(${PROJECT_NAME})

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/sfp/sfp_transmitter.pio)
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/sfp/sfp_receiver.pio)

target_sources(${PROJECT_NAME} PRIVATE
        ./src/main.cpp
        ./src/board/board.cpp
        ./src/sfp/sfp_transmitter.cpp
        ./src/sfp/sfp_receiver.cpp
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        pico_stdlib
        pico_multicore
        hardware_pio
        hardware_dma
        hardware_irq
        hardware_i2c
        )

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Flash ROM上にCodeを配置すると意図しないタイミングでWaitが入る場合があるため対処
# https://forums.raspberrypi.com/viewtopic.php?t=359614
#pico_set_binary_type(${PROJECT_NAME} no_flash)
pico_set_binary_type(${PROJECT_NAME} copy_to_ram)

pico_add_extra_outputs(${PROJECT_NAME})

