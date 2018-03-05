#include "N64_gamepad_usb.h"
#include "multiplayer.h"

const uint8_t N64_PIN1 = 2;
const uint8_t N64_PIN2 = 3;

N64_hid p1(1, N64_PIN1);
N64_hid p2(2, N64_PIN2);

gamepad::multiplayer<N64_hid> multi(p1,p2);

void setup() {}

void loop() {
  multi.read();

  multi.players.get(0)->print();
  Serial.print("\t"); Serial.print(p1.N64_pin_bit,BIN); Serial.print("\t"); Serial.print(p1.N64_pin); Serial.print("\t");
  
  multi.players.get(1)->print();
  Serial.print("\t"); Serial.print(p2.N64_pin_bit,BIN); Serial.print("\t"); Serial.print(p2.N64_pin); Serial.println("\t");

  //p1.read();
  //p2.read();
}