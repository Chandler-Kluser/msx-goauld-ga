# MSX Goa'uld Guardian Angel

> **⚠️🚨 Warning 🚨⚠️**  
> For firmware 080 check master branch

MSX Goa'uld Guardian Angel for USB Gamepad use.

Made for [RP2040 Zero](https://www.waveshare.com/wiki/RP2040-Zero) and [Raspberry Pi Pico](https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html#pico-1-family) boards.

## Wiring

| Device                   | Function   | Pin       | Connection Note                  |
|--------------------------|------------|-----------|----------------------------------|
| Tang Nano 20k UART RX    | UART RX    | 79        | Connect to TX of RP2040          |
| RP2040 UART TX           | UART TX    | GPIO 0    | Connect to RX of Tang Nano 20k   |

- 090 and 080 wirings are not compatible
- Both FPGA and RP2040 must have compatible firmwares together
- Make sure to power Raspberry Pi Pico to MSX 5V power supply to Vout (or Vbus) pin

## Build Instructions

You will need to clone [tinyusb](https://github.com/hathach/tinyusb) and [Raspberry Pi Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk).

For [Raspberry Pi Pico](https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html#pico-1-family), clone this repository and run:

```bash
cd msx-goauld-ga
mkdir build && cd build && cmake .. && make -j$(nproc)
```

For [RP2040 Zero](https://www.waveshare.com/wiki/RP2040-Zero), define `-DRP2040_ZERO` directive in cmake:

```bash
cd msx-goauld-ga
mkdir build && cd build && cmake -DRP2040_ZERO=1 .. && make -j$(nproc)
```

## Some ideas

- [ ] Scanlines toggle button
- [ ] OSD toggle button
- [ ] USB Keyboard basic functionalities
- [ ] USB Keyboard
- [ ] Local Firmware Switching
- [ ] Persistent Configuration Files
- [x] USB Gamepad
- [ ] Gamepad Left Analog to Digital Support
- [ ] Gamepad Remap Keys
- [ ] Gamepad Add Profiles for Keymappings
- [ ] Gamepad Turbo/Autofire
- [ ] Reset button (needs MSX external mod in reset wire, WIP)
- [ ] USB Floppy Drives
- [ ] USB Mass Storage
- [ ] Ethernet USB adapters
- [ ] WiFi USB adapters
- [ ] BIOS Switching

## Acknowledgements

Big thanks to:

- [tinyusb](https://github.com/hathach/tinyusb) developers
- [tusb_input](https://github.com/Ryzee119/tusb_xinput) XInput Driver based on tinyusb
