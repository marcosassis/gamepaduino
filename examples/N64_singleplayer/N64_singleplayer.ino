#include "gamepad.h"
#include "gamepad_joystick_hid.h"

#define _GAMEPAD_DEFINE_N64_HID
#include "N64_gamepad.h"


const uint8_t N64_PIN = 3;

N64_hid p1(1, N64_PIN);

void setup() {
}

void loop() {
  p1.read();
}
