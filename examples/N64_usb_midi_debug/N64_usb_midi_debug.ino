#include "midi_luthier_proto.h"
#include "midi_instrument.h"
#include "midi_usb_interface.h"
#include "N64_gamepad.h"
using namespace gamepad;
using namespace meta::midi::luthier;

// available pins for N64 controller interface
// on 'pro micro' (ATmega32U4): 2,3,4,6 [tested]
const uint8_t N64_PIN1 = 2;
const uint8_t N64_PIN2 = 3;

// ID values: 1 to 4 (tested on pro micro (ATmega32U4))
N64_gamepad p1(1, N64_PIN1);
//N64_hid p2(2, N64_PIN2);
// tested [so far] up to 2 N64 gamepads simultaneously (same 32U4)
// `-> more 2 SNES gamepads plugged on another pro micro on same PC
//     `-> the IDs still had to be chosen different, even on 2 boards
//         that is because we use these IDs to choose USB-HID IDs

//N64_multiplayer multi(p1,p2);

/*
template<typename T>
T* count_up_to(T* init_this_array,  T up2) {
  for(T i=0; i<up2; ++i)
    init_this_array[i] = i;
  return init_this_array;
}
*/

// midi interface prototype

typedef midi_instrument<N64_gamepad>  N64_midi;
typedef pitch_wheel<N64_midi>         N64_bender;        
using   meta::midi::midi_usb_interface;

/*
  //        0         1         2     3       4     5       6       7
  enum bid {A,        B,        Z,    start,  Dup,  Ddown,  Dleft,  Dright,
  //        8         9         10    11      12    13      14      15
            reset,   unkown,    L,    R,      Cup,  Cdown,  Cleft,  Cright};
*/
midi_usb_interface            midiusb;
N64_midi::note_t   note_map[N64_gamepad::N_BUTTONS]={0,1,2,3,4,5,6,7,16,9,10,11,12,13,14,15};
midi_data_t                                      basetone=69; // A4
N64_midi           p1midi(p1, midiusb, note_map, basetone);
N64_bender         p1bender(p1midi);


void setup() {
  Serial.begin(9600);
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
  //p1midi.read();
  p1bender.read();
  midiusb.flush();
}