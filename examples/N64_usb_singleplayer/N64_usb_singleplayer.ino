#define _GAMEPAD_N64_SINGLEPLAYER // optional define, singleplayer will work anyway
#include "N64_gamepad_usb.h"

const uint8_t N64_PIN = 3;

N64_hid p1(1, N64_PIN);

void setup() {}

void loop() {
  p1.read();
}