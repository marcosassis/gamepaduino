#define _GAMEPAD_SNES_SINGLEPLAYER // optional define, singleplayer will work anyway
#include "SNES_gamepad_usb.h"

const uint8_t CLOCK_PIN = 9;
const uint8_t LATCH_PIN = 8;
const uint8_t DATA_PIN = 7;

SNES_hid p1(1, DATA_PIN, CLOCK_PIN, LATCH_PIN);

void setup() {}

void loop() {
  p1.read();
}
