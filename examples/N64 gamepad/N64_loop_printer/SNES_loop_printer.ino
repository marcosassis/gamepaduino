// #include "SNES_gamepad_usb.h" // if your board is USB enabled you can test this
#include "SNES_gamepad.h"
// this defines multiplayer and gamepad interfaces/classes
// and implementation for SNES gamepad protocol

// using gamepad;
// using gamepad::SNES_gamepad;
// using gamepad::SNES_multiplayer;

const uint8_t CLOCK_PIN = 9;
const uint8_t LATCH_PIN = 8;
const uint8_t DATA_PIN1 = 7;
const uint8_t DATA_PIN2 = 6;
// any digital pin will work for this SNES gamepad interface
// tested so far up to 4 gamepads simultaneously

// if your board is USB enabled you can test these // print AND joystick USB HID
//SNES_hid p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
//SNES_hid p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);
// available IDs are 1..4 [for USB HID]

gamepad::SNES_gamepad p1(1, DATA_PIN1, CLOCK_PIN, LATCH_PIN);
gamepad::SNES_gamepad p2(2, DATA_PIN2, CLOCK_PIN, LATCH_PIN);
// [available IDs are any [general]

gamepad::SNES_multiplayer multi(p1, p2); // just comment p2 and multiplayer stuff you are singleplayer

void setup() {
  Serial.begin(9600);
}

const int VERBOSE_LEVEL = 3;
  /**
    verbose:
    0: print buttonset buttons as bits
    1: same as before, but with spaces between bytes
    2: print all '{ button_name: button_state; ...}'
    3: print all 'button_name:\t\tbutton_state\n'
  */

void loop() {
  multi.read();

  //p1.read(); // call this if you are singleplayer
  //p2.read();
  // if multi, don't call these directly, mutiplayer class already does it on it's read()

  //multi.players.get(0)->print(); Serial.print("\t");
  //multi.players.get(1)->print(); Serial.println();

  if(VERBOSE_LEVEL>=3)
    Serial.print("p1:\n");
  else
    Serial.print("p1:\t");
  
  p1.print(VERBOSE_LEVEL); // print p1
  
  if(VERBOSE_LEVEL>=2)
    Serial.print("\n");
  else
    Serial.print("\t");
    
  if(VERBOSE_LEVEL>=3)
    Serial.print("p2:\n");
  else
    Serial.print("p2:\t");
  
  p2.print(VERBOSE_LEVEL); // print p2
  
  if(VERBOSE_LEVEL>=2)
    Serial.print("\n\n");
  else
    Serial.print("\n");

}
