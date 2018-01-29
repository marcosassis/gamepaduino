//#include "gamepad.h"
#include "gamepad_joystick_hid.h"

const uint8_t CLOCK_PIN = 9;//4;
const uint8_t LATCH_PIN = 8;//5;
const uint8_t DATA_PIN = 7;

//SNES_gamepad p1(1, DATA_PIN, CLOCK_PIN, LATCH_PIN);
SNES_hid p1(1, DATA_PIN, CLOCK_PIN, LATCH_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("hello");
}


void loop() {

  p1.read();

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
