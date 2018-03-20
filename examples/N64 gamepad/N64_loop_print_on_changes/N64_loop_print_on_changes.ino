// see also N64_loop_print
#include "N64_gamepad.h"

const uint8_t N64_PIN1 = 2;
const uint8_t N64_PIN2 = 3;

gamepad::N64_gamepad p1(1, N64_PIN1);
gamepad::N64_gamepad p2(2, N64_PIN2);

gamepad::N64_multiplayer multi(p1, p2);

void setup() {
  Serial.begin(9600);
}

const uint8_t START = gamepad::N64_gamepad::bid::start;

void loop() {
  multi.read(); // everything updates on read()
  
  // use these button_state_has_changed tests only when testing few buttons
  if(p1.button_state_has_changed(START))
    if(p1.button_is_pressed(START)) // only on rise changes
      Serial.println("\t\thello p1");
  
  if(p2.button_state_has_changed(START))
    if(p1.button_is_pressed(START))      // do you see?? ;)
      Serial.println("\t\thello p2");

  // use this to take action about many buttons
  if(p1.any_state_has_changed()) {
    Serial.print(micros()); Serial.print("\tp1:\t");
    p1.println(3); // verbose=2: print all '{ button_name: button_state, ... }'
  }
  
  if(p2.any_state_has_changed()) {
    Serial.print(micros()); Serial.print("\tp2:\t");
    p2.println(3);
  }
}