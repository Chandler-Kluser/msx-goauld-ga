# MSX Goa'uld Friend

The MSX Goa'uld Guardian Angel for hard tasks. Made with [RP2040 Zero](https://www.waveshare.com/wiki/RP2040-Zero). 

Make sure to power Raspberry Pi Pico to MSX 5V power supply to Vout pin.

## Wiring

| Device                   | Function   | Pin       | Connection Note                  |
|--------------------------|------------|-----------|----------------------------------|
| Tang Nano 20k UART TX    | UART TX    | 79        | Connect to RX of RP2040          |
| Tang Nano 20k UART RX    | UART RX    | 80        | Connect to TX of RP2040          |
| RP2040 UART TX           | UART TX    | GPIO 0    | Connect to RX of Tang Nano 20k   |
| RP2040 UART RX           | UART RX    | GPIO 1    | Connect to TX of Tang Nano 20k   |

*Note*: built for [waveshare RP2040 Zero](https://www.waveshare.com/w/upload/2/2b/RP2040-Zero-details-7.jpg), builtin LED is GPIO16. Both FPGA and RP2040 must compatible firmwares together.

## Build Instructions

Before build, make sure to:

* have `pico-sdk` with `tinyusb` library
* check `PICO_SDK_PATH` environment variable linked to `pico-sdk` directory

```
mkdir build && cd build
rm -rf * && cmake -DFAMILY=rp2040 -DBOARD=raspberry_pi_pico -G Ninja .. && cmake --build .
```

## Some ideas

- [x] Scanlines toggle button
- [x] OSD toggle button
- [x] USB Keyboard basic functionalities
- [x] USB Keyboard
- [ ] USB Gamepad
- [ ] Reset button (needs MSX external mod in reset wire, WIP)
- [ ] USB Floppy Drives
- [ ] USB Mass Storage
- [ ] Ethernet USB adapters
- [ ] Bluetooth USB adapters
- [ ] WiFi USB adapters
- [ ] BIOS Switching
- [ ] TF Support
- [ ] OSD Control over PSG default control ports pins (needs OSD dedicated button)