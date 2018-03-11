#include "midi_instrument.h"
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
//SNES_gamepad p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);

//SNES_multiplayer multi(p1,p2);

/*
template<typename T>
T* count_up_to(T* init_this_array,  T up2) {
  for(T i=0; i<up2; ++i)
    init_this_array[i] = i;
  return init_this_array;
}
*/

/// midi interface prototype

// this uses an 'interval map + tone' method for playing MIDI notes as buttons are pressed
typedef   midi_instrument<SNES_gamepad>   SNES_midi;

// this is midi_interface implemented (only send for now) for MIDIUSB (library adapter)
using     meta::midi::midi_usb_interface;

//// SNES buttons ids
////        0  1  2       3      4   5     6     7      8  9  10 11
//enum bid {B, Y, select, start, up, down, left, right, A, X, L, R};
int8_t note_map[SNES_gamepad::N_BUTTONS]={0,1,2,3,4,5,6,7,8,9,10,11};

// initialize everybody
midi_data_t                                       basetone=69; // A4
midi_usb_interface             midiusb;
SNES_midi           p1midi(p1, midiusb, note_map, basetone);

void setup() {
  //Serial.begin(9600);
}

void loop() {
  /*
  p1.read();
  if(p1.button_state_has_changed(SNES_gamepad::bid::B)) {
    if(p1.button_is_pressed(SNES_gamepad::bid::B))
      midiusb.note_on(0, 69, 127);
    else
      midiusb.note_off(0, 69, 127);
    
    for(uint8_t i=0; i<4; ++i) {
      Serial.print(midiusb.event.data[i],HEX); Serial.print("\t");
    }
    Serial.println();
  }
  */
  p1midi.read();
  midiusb.flush();
}