#include "N64_gamepad_usb.h"

const uint8_t N64_PIN1 = 2;
const uint8_t N64_PIN2 = 3;
// available pins on pro micro (ATmega32U4): 2,3,4,6 [tested]

// IDs values: 1 to 4 (tested on pro micro (ATmega32U4))
N64_hid p1(1, N64_PIN1);
N64_hid p2(2, N64_PIN2);
// tested [so far] up to 2 N64 gamepads simultaneously (same 32U4)
// `-> more 2 SNES gamepads plugged on another pro micro on same PC
//     `-> IDs still had to be chosen different, even on 2 boards
//         that is because we use these IDs to choose USB-HID IDs

N64_multiplayer multi(p1,p2);

void setup() {}

void loop() {
  multi.read();
}