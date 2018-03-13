// see also SNES_loop_print
#include "SNES_gamepad.h"

const uint8_t CLOCK_PIN = 9;
const uint8_t LATCH_PIN = 8;
const uint8_t DATA_PIN1 = 7;
const uint8_t DATA_PIN2 = 6;

gamepad::SNES_gamepad p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
gamepad::SNES_gamepad p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);

gamepad::SNES_multiplayer multi(p1, p2);

void setup() {
  Serial.begin(9600);
}

const uint8_t START = gamepad::SNES_gamepad::bid::start;
const uint8_t SELECT = gamepad::SNES_gamepad::bid::select;

void loop() {
  multi.read(); // everything updates on read()
  
  // use these button_state_has_changed tests only when testing few buttons
  if(p1.button_state_has_changed(START))
    if(p1.button_is_pressed(START)) // only on rise changes
      Serial.println("\t\thello p1");
  
  if(p2.button_state_has_changed(SELECT))
    if(p1.button_is_pressed(SELECT))      // do you see?? ;)
      Serial.println("\t\thello p2");

  // use this to take action about many buttons
  if(p1.any_button_state_has_changed()) {
    Serial.print(micros()); Serial.print("\tp1:\t");
    p1.println(2); // verbose=2: print all '{ button_name: button_state, ... }'
  }
  
  if(p2.any_button_state_has_changed()) {
    Serial.print(micros()); Serial.print("\tp2:\t");
    p2.println(2);
  }
}