cmake_minimum_required(VERSION 3.20)

include(src/pico_sdk_import.cmake)

project(msx-goauld-ga C CXX ASM)

pico_sdk_init()

# Define RP2040_ZERO as a cache variable
set(RP2040_ZERO OFF CACHE BOOL "Enable RP2040_ZERO support")
if(RP2040_ZERO)
    add_definitions(-DRP2040_ZERO=1)
endif()

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tusb_xinput_host.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/xinput_host.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/od_tx_logic.c
)

pico_generate_pio_header(msx-goauld-ga ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pico_stdlib
    tinyusb_host
    hardware_pio
    tinyusb_board
    pico_multicore
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og")
else()
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
endif()

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})
