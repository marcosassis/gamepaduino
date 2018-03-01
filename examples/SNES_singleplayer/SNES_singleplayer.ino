#include "gamepad.h"
#include "gamepad_joystick_hid.h"

#define _GAMEPAD_DEFINE_SNES_HID
#include "SNES_gamepad.h"


const uint8_t CLOCK_PIN = 9;
const uint8_t LATCH_PIN = 8;
const uint8_t DATA_PIN = 7;

SNES_hid p1(1, DATA_PIN, CLOCK_PIN, LATCH_PIN);

void setup() {
}

void loop() {
  p1.read();
}
