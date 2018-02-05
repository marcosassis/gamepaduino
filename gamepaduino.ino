//#include "multiplayer.h"
#include "SNES_gamepad.h"
//#include "gamepad.h"
//#include "gamepad_joystick_hid.h"
//#include "gamepad_midi.h"
#define _METAPAD_DEBUG
#include "metapad.h"

const uint8_t CLOCK_PIN = 9;//4;
const uint8_t LATCH_PIN = 8;//5;
const uint8_t DATA_PIN1 = 7;
const uint8_t DATA_PIN2 = 6;

SNES_gamepad p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
SNES_gamepad p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);
//SNES_hid p1(1, DATA_PIN, CLOCK_PIN, LATCH_PIN);

//                      0  1  2       3      4   5     6     7      8  9  10 11
//              enum b {B, Y, select, start, up, down, left, right, A, X, L, R};
const int8_t blues[] = {3, 10, -128, -128, -128, -128, -128, -128, 5, 7, 0, 6}; 
metapad<SNES_gamepad> p1midi(p1, blues, 69, 0, 100); // 69=A4 ref.: midi specs
metapad<SNES_gamepad> p2midi(p2, blues, 69, 1, 64); // 69=A4 ref.: midi specs

SNES_multiplayer mp(p1midi, p2midi);


void setup() {
  
  //mp.players.add(&p2midi);

#ifdef _METAPAD_DEBUG
  Serial.begin(9600);
  Serial.println("hello"); // don't know why never can see this
#endif
}

void loop() {

  //p1midi.read();
  //p2midi.read();
  mp.read();

/*
  for (uint8_t i = 0; i < p1.get_n_buttons(); ++i) {

    if (p1.button_state_has_changed(i)) {

      Serial.print(i);
      Serial.print("\t:\t");
      Serial.print(p1.get_button_names()[i]);
      Serial.print("\t:\t");
      Serial.print(p1.get_button_state(i));
      Serial.print("\t:\t");
      Serial.print(p1.button_state_has_changed(i));
      Serial.print("\t:\t");
      Serial.print(p1.buttons_changed_mask(), BIN);
      Serial.print("\t:\t");
      Serial.print(p1.get_buttons_last(), BIN);
      Serial.print("\t:\t");
      Serial.println(p1.get_buttons(), BIN);

    }//end if(p1.button_state_has_changed(i))

  }//end for
  */
}
