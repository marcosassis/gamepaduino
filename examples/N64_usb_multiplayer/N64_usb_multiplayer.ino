#include "N64_gamepad_usb.h"

const uint8_t N64_PIN1 = B00000010; // bit for pin 2 on pro micro
const uint8_t N64_PIN2 = B00000001; // bit for pin 3 on pro micro
// https://www.arduino.cc/en/Hacking/PinMapping32u4

N64_hid p1(1, N64_PIN1);
N64_hid p2(2, N64_PIN2);

void setup() {}

void loop() {
  p1.read();
  //Serial.print(int(PIND),HEX); Serial.print("\t");
  //p1.print();
  //Serial.print("\t\t");
  p2.read();
  //Serial.print(int(PIND),HEX); Serial.print("\t");
  //p2.print();
  //Serial.println();
}