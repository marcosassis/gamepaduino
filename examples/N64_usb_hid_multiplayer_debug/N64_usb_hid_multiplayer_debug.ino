#include "N64_gamepad_usb.h"
// see other *_debug.ino for other things you can do with this library

const uint8_t N64_PIN1 = 2;
const uint8_t N64_PIN2 = 3;
// available pins on pro micro [tested]: 2,3,4,6 

N64_hid p1(1, N64_PIN1);
N64_hid p2(2, N64_PIN2);

N64_multiplayer multi(p1,p2);

void setup() {}

void loop() {
  multi.read();

  multi.players.get(0)->print(); Serial.print("\t");
  multi.players.get(1)->print(); Serial.println();
  
  p1.print(); Serial.print("\t");
  p2.print(); Serial.println("\n");

  //p1.read();
  //p2.read();
  // don't call these directly, mutiplayer class already does it on it's read()
}