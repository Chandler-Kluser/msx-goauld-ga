#include "hardware/gpio.h"
#include "bsp/board_api.h"
#include "hardware/uart.h"
#include "config.h"

extern volatile uint8_t shared_buffer;

void tx_task_od() {
    while (1) {
        // shared_buffer must be inverted because of:
        // 1) Two Most Significant bits in PSG port are kept high
        // 2) Remaining bits in PSG port are active low
        uart_putc_raw(UART_ID, ~shared_buffer);

        if (shared_buffer & A_MASK) {
            #ifdef RP2040_ZERO
                put_pixel(urgb_u32(64, 0, 0));
            #else
                gpio_put(LED_PIN, 1);
            #endif
        } else {
            #ifdef RP2040_ZERO
                put_pixel(urgb_u32(0, 0, 0));
            #else
                gpio_put(LED_PIN, 0);
            #endif
        }
        sleep_ms(1);
    }
}
