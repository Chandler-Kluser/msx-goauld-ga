#ifndef CONFIG_H
#define CONFIG_H

// Shared Buffer Logic:
//
// D7 D6 D5 D4 D3 D2 D1 D0                            | Pressed | Released |
// |  |  |  |  |  |  |  +------> D-PAD UP    BUTTON   |    1    |     0    |
// |  |  |  |  |  |  +---------> D-PAD DOWN  BUTTON   |    1    |     0    |
// |  |  |  |  |  +------------> D-PAD LEFT  BUTTON   |    1    |     0    |
// |  |  |  |  +---------------> D-PAD RIGHT BUTTON   |    1    |     0    |
// |  |  |  +----------------------------> A BUTTON   |    1    |     0    |
// |  |  +-------------------------------> B BUTTON   |    1    |     0    |
// |  +----------------------------------> X BUTTON   |    1    |     0    |
// +-------------------------------------> Y BUTTON   |    1    |     0    |

#define DPAD_UP_MASK    0b00000001
#define DPAD_DOWN_MASK  0b00000010
#define DPAD_LEFT_MASK  0b00000100
#define DPAD_RIGHT_MASK 0b00001000
#define A_MASK          0b00010000
#define B_MASK          0b00100000
#define X_MASK          0b01000000
#define Y_MASK          0b10000000

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#ifdef RP2040_ZERO
    #include <stdint.h>
    #define LED_PIN 16
    void put_pixel(uint32_t pixel_grb);
    uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
#else
    #define LED_PIN 25
#endif

#endif // CONFIG_H
