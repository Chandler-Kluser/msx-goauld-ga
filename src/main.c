#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include <pico/multicore.h>

#include "bsp/board_api.h"
#include "tusb.h"
#include "config.h"

#ifdef RP2040_ZERO
#include "hardware/pio.h"
#include "ws2812.pio.h"

void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
}
#endif

// Shared SRAM buffer
__attribute__((section(".ram")))
volatile uint8_t shared_buffer = 0;

extern void tx_task_od();

int main(void) {
    board_init();

    // initialise UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

#ifdef RP2040_ZERO
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, LED_PIN, 800000, false);
#else
// initialize LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
#endif

    // init host stack on configured roothub port
    tusb_rhport_init_t host_init = {
        .role = TUSB_ROLE_HOST,
        .speed = TUSB_SPEED_AUTO
    };

    tusb_init(BOARD_TUH_RHPORT, &host_init);

    // endless task for Core 1
    multicore_launch_core1(tx_task_od);

    while (1) tuh_task();

}
