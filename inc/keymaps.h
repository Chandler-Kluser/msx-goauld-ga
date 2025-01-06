#ifndef  KEYMAPS_H
#define KEYMAPS_H

const uint8_t keycode_to_goauld[128] = {
    // Letters (a-z)
    [0x04] = 128+16*6+2, // Key 'a'
    [0x05] = 128+16*7+2, // Key 'b'
    [0x06] = 128+16*0+3, // Key 'c'
    [0x07] = 128+16*1+3, // Key 'd'
    [0x08] = 128+16*2+3, // Key 'e'
    [0x09] = 128+16*3+3, // Key 'f'
    [0x0A] = 128+16*4+3, // Key 'g'
    [0x0B] = 128+16*5+3, // Key 'h'
    [0x0C] = 128+16*6+3, // Key 'i'
    [0x0D] = 128+16*7+3, // Key 'j'
    [0x0E] = 128+16*0+4, // Key 'k'
    [0x0F] = 128+16*1+4, // Key 'l'
    [0x10] = 128+16*2+4, // Key 'm'
    [0x11] = 128+16*3+4, // Key 'n'
    [0x12] = 128+16*4+4, // Key 'o'
    [0x13] = 128+16*5+4, // Key 'p'
    [0x14] = 128+16*6+4, // Key 'q'
    [0x15] = 128+16*7+4, // Key 'r'
    [0x16] = 128+16*0+5, // Key 's'
    [0x17] = 128+16*1+5, // Key 't'
    [0x18] = 128+16*2+5, // Key 'u'
    [0x19] = 128+16*3+5, // Key 'v'
    [0x1A] = 128+16*4+5, // Key 'w'
    [0x1B] = 128+16*5+5, // Key 'x'
    [0x1C] = 128+16*6+5, // Key 'y'
    [0x1D] = 128+16*7+5, // Key 'z'

    // Numbers (1-0)
    [0x27] = 128+16*0+0, // Key '0'
    [0x1E] = 128+16*1+0, // Key '1'
    [0x1F] = 128+16*2+0, // Key '2'
    [0x20] = 128+16*3+0, // Key '3'
    [0x21] = 128+16*4+0, // Key '4'
    [0x22] = 128+16*5+0, // Key '5'
    [0x23] = 128+16*6+0, // Key '6'
    [0x24] = 128+16*7+0, // Key '7'
    [0x25] = 128+16*0+1, // Key '8'
    [0x26] = 128+16*1+1, // Key '9'

    // Symbols
    [0x2C] = 128+16*0+8,  // Spacebar   == MSX Spacebar
    [0x2D] = 128+16*2+1,  // minus      == MSX -
    [0x2E] = 128+16*5+2,  // plus/equal == MSX = (when SHIFT_UP)
    [0x2F] = 128+16*4+2,  // [          == MSX ¥ or backslash
    [0x30] = 128+16*5+1,  // ]          == MSX @
    [0x31] = 128+16*4+1,  // Slash /    == MSX / or ?
    [0x56] = 128+16*4+1,  // Slash /    == MSX /
    [0x36] = 128+16*2+2,  // Comma      == MSX comma
    [0x37] = 128+16*3+2,  // Period     == MSX .
    [0x34] = 128+16*0+2,  // "          == MSX ` (or ")
    [0x33] = 128+16*7+1,  // ;          == MSX ;
    [0x38] = 128+16*4+2,  // / or ?     == MSX / or ?

    // Function Keys (F1-F12)
    [0x3A] = 128+16*5+6,  // F1  == MSX F1
    [0x3B] = 128+16*6+6,  // F2  == MSX F2
    [0x3C] = 128+16*7+6,  // F3  == MSX F3
    [0x3D] = 128+16*0+7,  // F4  == MSX F4
    [0x3E] = 128+16*1+7,  // F5  == MSX F5
    [0x3F] = 128+16*4+7,  // F6  == MSX STOP BUTTON
    [0x40] = 128+16*2+6,  // F7  == MSX GRAPH BUTTON
    [0x41] = 1,           // F8  == OSD toggle
    [0x42] = 1,           // F9  == OSD toggle
    [0x43] = 2,           // F10 == SHIFT_TOGGLE
    [0x44] = 0,           // F11 == SCANLINES toggle
    [0x45] = 3,           // F12 == RESET

    // Arrows
    [0x52] = 128+16*5+8, // Keyboard UpArrow
    [0x51] = 128+16*6+8, // Keyboard DownArrow
    [0x50] = 128+16*4+8, // Keyboard LeftArrow
    [0x4F] = 128+16*7+8, // Keyboard RightArrow

    // Control Keys
    [0x2A] = 128+16*5+7, // Backspace == MSX Backspace
    [0x4C] = 128+16*3+8, // Delete == MSX Delete
    [0x28] = 128+16*7+7, // Enter == MSX Return
    [0x29] = 128+16*2+7, // ESCAPE == MSX Escape
    [0x2B] = 128+16*3+7, // Tab == MSX TAB
    [0x4A] = 128+16*1+8, // Home == MSX HOME
};

#endif