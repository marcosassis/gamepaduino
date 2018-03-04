#include "N64_gamepad_usb.h"

const uint8_t N64_PIN1 = 2;
const uint8_t N64_PIN2 = 3;

N64_hid p1(1, N64_PIN1);
N64_hid p2(2, N64_PIN2);

void setup() {}

void loop() {
  p1.read();
  p2.read();
}