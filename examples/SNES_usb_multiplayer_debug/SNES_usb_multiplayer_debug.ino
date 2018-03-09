#include "SNES_gamepad_usb.h"
// this defines multiplayer and gamepad interfaces/classes
// and my implementation for SNES gamepad protocol

// see other *_debug.ino for other things you can do with this library

const uint8_t CLOCK_PIN = 9;
const uint8_t LATCH_PIN = 8;
const uint8_t DATA_PIN1 = 7;
const uint8_t DATA_PIN2 = 6;
// any digital pin will work for this SNES gamepad interface
// tested so far up to 4 gamepads simultaneously

SNES_hid p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
SNES_hid p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);
//gamepad::SNES_gamepad p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
//gamepad::SNES_gamepad p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);

SNES_multiplayer multi(p1,p2);
//gamepad::multiplayer<gamepad::SNES_hid> multi(p1,p2);
//gamepad::multiplayer<gamepad::SNES_gamepad> multi(p1,p2);


void setup() {
  //Serial.begin(9600);
}

void loop() {
  multi.read();
  
  //multi.players.get(0)->print(); Serial.print("\t");
  //multi.players.get(1)->print(); Serial.println();
  
  p1.print(); Serial.print("\t");
  p2.print(); Serial.println();//("\n");

  //p1.read();
  //p2.read();
  // don't call these directly, mutiplayer class already does it on it's read()
}