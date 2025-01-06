#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/timer.h"
#include "bsp/board_api.h"
#include "tusb_config.h"
#include "class/hid/hid_host.h"

// ============== Defines ==============

#define KEYBOARD_REPORT_LEN 6
// WS2812 LED FOR RP2040 ZERO
#define LED_PIN 16
// LED for Raspberry Pi Pico
// #define LED_PIN 25
#define BUF_COUNT   4

// ================= headers ===================

void led_blinking_task(void);
void print_device_descriptor(tuh_xfer_t* xfer);

// ============== Global Variables ==============

tusb_desc_device_t desc_device;
uint8_t buf_pool[BUF_COUNT][64];
uint8_t buf_owner[BUF_COUNT] = { 0 };
uint8_t isMounted = 0;
uint8_t kb_leds = 0;
uint8_t kb_modifiers = 0;
uint8_t kb_keys[120] = {0};
uint8_t SHIFT_UP = 0;

// ============== Function to run on Core 1 ==============

void core1_entry() {
    while (1) {
        // gpio_put(LED_PIN, 1);  // Turn LED on
        sleep_ms(200);         // Wait 500 ms
        // gpio_put(LED_PIN, 0);  // Turn LED off
        // sleep_ms(800);         // Wait 500 ms
    }
}

// ============== Timer Callback ==============

bool toggle_led_callback(repeating_timer_t *timer) {
    gpio_put(LED_PIN, isMounted);
    return true;
}

// ============== Main ==============

int main() {
    stdio_init_all();

    tusb_init();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Launch Core 1
    multicore_launch_core1(core1_entry);

    repeating_timer_t timer;
    add_repeating_timer_ms(5, toggle_led_callback, NULL, &timer);

#ifdef DEBUG
    printf("Firmware Boot Done\n");
#endif

    // Firmware boot LED output
    gpio_put(LED_PIN, 1);
    sleep_ms(200);
    gpio_put(LED_PIN, 0);

    tuh_hid_set_default_protocol(HID_PROTOCOL_REPORT);

    while (1) {
        tuh_task();
    }

    return 0;
}