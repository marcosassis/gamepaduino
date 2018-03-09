#include "SNES_gamepad_usb.h"

// any digital pin will work for this SNES gamepad interface
const uint8_t CLOCK_PIN = 9;
const uint8_t LATCH_PIN = 8;
const uint8_t DATA_PIN1 = 7;
const uint8_t DATA_PIN2 = 6;

// ID values: 1 to 4 (tested on pro micro (ATmega32U4))
SNES_hid p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
SNES_hid p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);
// tested [so far] up to 4 SNES gamepads simultaneously (same 32U4)

SNES_multiplayer multi(p1,p2);

void setup() {}

void loop() {
  multi.read();
}