#include "SNES_gamepad_usb.h"

const uint8_t CLOCK_PIN = 9;
const uint8_t LATCH_PIN = 8;
const uint8_t DATA_PIN1 = 7;
const uint8_t DATA_PIN2 = 6;
// any digital pin will work for this SNES gamepad interface
// tested so far up to 4 gamepads simultaneously

SNES_hid p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
SNES_hid p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);

gamepad::multiplayer<SNES_hid> multi(p1,p2); // naïve implementation (working)

void setup() {}

void loop() {
  multi.read();
}
