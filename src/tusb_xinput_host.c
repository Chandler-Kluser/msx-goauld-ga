#include "bsp/board_api.h"
#include "tusb.h"
#include "xinput_host.h"
#include "hardware/gpio.h"
#include "config.h"

#ifdef RP2040_ZERO
extern void put_pixel(uint32_t pixel_grb);
#endif

extern volatile uint8_t shared_buffer;
static uint8_t prev_wButtons = 0;

const uint16_t button_mask = XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_DOWN |
                            XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT |
                            XINPUT_GAMEPAD_A | XINPUT_GAMEPAD_B |
                            XINPUT_GAMEPAD_X | XINPUT_GAMEPAD_Y;

usbh_class_driver_t const* usbh_app_driver_get_cb(uint8_t* driver_count) {
    *driver_count = 1;
    return &usbh_xinput_driver;
}

void tuh_xinput_report_received_cb(uint8_t dev_addr, uint8_t instance, xinputh_interface_t const* xid_itf, uint16_t len) {
    const xinput_gamepad_t *p = &xid_itf->pad;

    if (xid_itf->last_xfer_result == XFER_RESULT_SUCCESS) {
        if (xid_itf->connected && xid_itf->new_pad_data) {
            // Get current button states
            uint16_t current_buttons = p->wButtons & button_mask;
            // Map buttons to shared_buffer (1 = pressed, 0 = released)
            shared_buffer = 0;
            if (current_buttons & XINPUT_GAMEPAD_DPAD_UP)    shared_buffer |= DPAD_UP_MASK;
            if (current_buttons & XINPUT_GAMEPAD_DPAD_DOWN)  shared_buffer |= DPAD_DOWN_MASK;
            if (current_buttons & XINPUT_GAMEPAD_DPAD_LEFT)  shared_buffer |= DPAD_LEFT_MASK;
            if (current_buttons & XINPUT_GAMEPAD_DPAD_RIGHT) shared_buffer |= DPAD_RIGHT_MASK;
            if (current_buttons & XINPUT_GAMEPAD_A)          shared_buffer |= A_MASK;
            if (current_buttons & XINPUT_GAMEPAD_B)          shared_buffer |= B_MASK;
            if (current_buttons & XINPUT_GAMEPAD_X)          shared_buffer |= X_MASK;
            if (current_buttons & XINPUT_GAMEPAD_Y)          shared_buffer |= Y_MASK;
            // Update previous button state
            prev_wButtons = current_buttons;
        }
    }

    // Continue receiving reports
    tuh_xinput_receive_report(dev_addr, instance);
}

void tuh_xinput_mount_cb(uint8_t dev_addr, uint8_t instance, const xinputh_interface_t *xinput_itf) {
    // If this is a Xbox 360 Wireless controller we need to wait for a connection packet
    // on the in pipe before setting LEDs etc. So just start getting data until a controller is connected.
    if (xinput_itf->type == XBOX360_WIRELESS && xinput_itf->connected == false) {
        tuh_xinput_receive_report(dev_addr, instance);
        return;
    }
    tuh_xinput_set_led(dev_addr, instance, 0, true);
    tuh_xinput_set_led(dev_addr, instance, 1, true);
    tuh_xinput_set_rumble(dev_addr, instance, 0, 0, true);
    tuh_xinput_receive_report(dev_addr, instance);
#ifdef RP2040_ZERO
    put_pixel(urgb_u32(255, 0, 0));
    sleep_ms(70);
    put_pixel(urgb_u32(0, 0, 0));
    sleep_ms(70);
    put_pixel(urgb_u32(0, 255, 0));
    sleep_ms(70);
    put_pixel(urgb_u32(0, 0, 0));
#else
    gpio_put(LED_PIN, 1);
    sleep_ms(70);
    gpio_put(LED_PIN, 0);
    sleep_ms(70);
    gpio_put(LED_PIN, 1);
    sleep_ms(70);
    gpio_put(LED_PIN, 0);
#endif
}

void tuh_xinput_umount_cb(uint8_t __attribute__((unused)) dev_addr, uint8_t __attribute__((unused)) instance) {
    gpio_put(LED_PIN, 0);
}

void tuh_hid_report_received_cb(uint8_t __attribute__((unused)) dev_addr, uint8_t __attribute__((unused)) instance, uint8_t __attribute__((unused)) const *report, uint16_t __attribute__((unused)) len) {

}
