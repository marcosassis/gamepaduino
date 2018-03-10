#include "midi_usb_interface.h"
#include "SNES_gamepad.h"
using namespace gamepad;

// any digital pin will work for this SNES gamepad interface
const uint8_t CLOCK_PIN = 9;
const uint8_t LATCH_PIN = 8;
const uint8_t DATA_PIN1 = 7;
const uint8_t DATA_PIN2 = 6;

// ID values: 1 to 4 (tested on pro micro (ATmega32U4))
SNES_gamepad p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
SNES_gamepad p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);
// tested [so far] up to 4 SNES gamepads simultaneously (same 32U4)

SNES_multiplayer multi(p1,p2);

meta::midi::midi_usb_interface midiusb;

void setup() {
  Serial.begin(9600);
}

void loop() {
  multi.read();
  if(p1.button_state_has_changed(SNES_gamepad::bid::B)) {
    if(p1.button_is_pressed(SNES_gamepad::bid::B))
      midiusb.note_on(0, 69, 127);
    else
      midiusb.note_on(0, 69, 127);
    
    for(uint8_t i=0; i<4; ++i) {
      Serial.print(midiusb.event[0],HEX); Serial.print("\t");
    }
    Serial.println();
  }
}