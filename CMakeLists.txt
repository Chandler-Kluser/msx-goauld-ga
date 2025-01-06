cmake_minimum_required(VERSION 3.17)

include(src/pico_sdk_import.cmake)

project(goauld_friend C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

pico_sdk_init()

set(FAMILY rp2040)

add_executable(goauld_friend
    src/main.c
    src/usbin.c
)

target_include_directories(goauld_friend PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${PICO_SDK_PATH}/src
)

target_link_libraries(goauld_friend
    pico_stdlib
    tinyusb_host
    tinyusb_board
    pico_multicore
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og")
    target_compile_definitions(goauld_friend PRIVATE DEBUG)
else()
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
endif()

target_compile_definitions(goauld_friend PRIVATE
    CFG_TUSB_MCU=OPT_MCU_RP2040
    PICO_PLATFORM=rp2040
    DEBUG_PORT=Serial1
    DEBUG_BAUDRATE=115200
    DEBUG_RP2040_PORT=DEBUG_PORT
)

pico_add_extra_outputs(goauld_friend)