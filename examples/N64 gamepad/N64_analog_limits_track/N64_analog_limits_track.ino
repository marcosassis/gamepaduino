#include "N64_gamepad_usb.h"

// available pins for N64 controller interface
// on 'pro micro' (ATmega32U4): 2,3,4,6 [tested]
const uint8_t N64_PIN1 = 2;
const uint8_t N64_PIN2 = 3;

// ID values: 1 to 4 (tested on pro micro (ATmega32U4))
N64_hid p1(1, N64_PIN1);
N64_hid p2(2, N64_PIN2);
// tested [so far] up to 2 N64 gamepads simultaneously (same 32U4)
// `-> more 2 SNES gamepads plugged on another pro micro on same PC
//     `-> the IDs still had to be chosen different, even on 2 boards
//         that is because we use these IDs to choose USB-HID IDs

N64_multiplayer multi(p1,p2);

void setup() {}

void loop() {
  multi.read();

  if(p1.any_state_has_changed()) {
    Serial.print(micros()); Serial.print("\tp1:\n\n");
    Serial.print("xmax: "); Serial.println(p1.get_max(0)); 
    Serial.print("xmin: "); Serial.println(p1.get_min(0)); 
    Serial.print("ymax: "); Serial.println(p1.get_max(1)); 
    Serial.print("ymin: "); Serial.println(p1.get_min(1)); 
    p1.println(3); // verbose=2: print all '{ button_name: button_state, ... }'
    Serial.println();
  }
  
  if(p2.any_state_has_changed()) {
    Serial.print(micros()); Serial.print("\tp2:\n\n");
    Serial.print("xmax: "); Serial.println(p2.get_max(0)); 
    Serial.print("xmin: "); Serial.println(p2.get_min(0)); 
    Serial.print("ymax: "); Serial.println(p2.get_max(1)); 
    Serial.print("ymin: "); Serial.println(p2.get_min(1)); 
    p2.println(3);
    Serial.println();
  }
}
